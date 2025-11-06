#include "physics.h"
#include "config.hpp"
#include "util/rand.hpp"
#include <box2d/box2d.h>
#include <spdlog/spdlog.h>

// defualt shape def
static b2ShapeDef dsd = [] {
	b2ShapeDef sd = b2DefaultShapeDef();
	sd.material = b2DefaultSurfaceMaterial();
	sd.material.friction = 0.0f;
	sd.density = 1.0f;
	return sd;
}();

Physics::Physics(PhysicsDep dep, std::shared_ptr<context::BallGroup> bg)
	: d(std::move(dep)), region(bg->region), bg(bg) {

	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = b2Vec2{0.0f, 0.0f};
	world = b2CreateWorld(&worldDef);

	createBrick();
	createWall();

	for (const auto &b : bg->list) {
		b2BodyId ball = createBall(b);
		bl.push_back(ball);
	}

	ballSize = static_cast<int>(bl.size());

	// spdlog::info("phy init {} {} {}", b->region, b->pos.x, b->pos.y);
}

Physics::~Physics() {
	spdlog::trace("phy destory {}", region);
	b2DestroyWorld(world);
}

bool Physics::contactCheck(b2ShapeId *shapeId) {

	auto body = b2Shape_GetBody(*shapeId);
	void *ud = b2Body_GetUserData(body);
	if (ud == nullptr) {
		return false;
	}

	bg->hit = true;

	context::Brick *b = (context::Brick *)ud;
	b->region = region;
	bg->power++;

	auto power =
		static_cast<float>(bg->power / ballSize) / config::powerAvg - 1.0f;
	power *= 0.2f;
	// spdlog::info("power {} {:.2f} {:.2f}", bg->power, config::powerAvg,
	// power);
	b->power = static_cast<double>(std::min(0.3f, std::max(power, -0.3f)));
	return true;
}

void Physics::update(float dt) {

	bg->hit = false;

	float x = d.misc->gamepadX;
	float y = d.misc->gamepadY;
	float gravity = std::sqrt(x * x + y * y);
	if (gravity > 1.0f) {
		x /= gravity;
		y /= gravity;
	}
	b2World_SetGravity(world, b2Vec2(x * config::gravity, y * config::gravity));

	float speed = d.misc->speed;
	if (speed < 1.0f) {
		_update(dt * speed);
	} else {
		for (int cnt = static_cast<int>(d.misc->speed); cnt > 0; cnt--) {
			_update(dt);
		}
	}
}

void Physics::_update(float deltaTime) {
	bg->power = 0;
	for (const auto &b : d.misc->brick) {
		b2BodyId bb = brick[b.id];
		if (b.region == region) {
			bg->power++;
			b2Body_Disable(bb);
		} else {
			b2Body_Enable(bb);
		}
	}

	// spdlog::info("brick {} power:{}", region, bg->power);

	b2World_Step(world, deltaTime, 1);
	b2ContactEvents ce = b2World_GetContactEvents(world);

	if (ce.beginCount > 0) {
		for (int i = 0; i < ce.beginCount; i++) {
			b2ContactBeginTouchEvent event = ce.beginEvents[i];
			contactCheck(&event.shapeIdA) || contactCheck(&event.shapeIdB);
		}
		// std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	for (int i = 0; i < ballSize; i++) {
		_updateBall(i);
	}
}

void Physics::_updateBall(int idx) {

	b2BodyId ballBody = bl[idx];

	b2Vec2 p = b2Body_GetPosition(ballBody);
	bg->list[idx]->pos = p;

	b2Vec2 v = b2Body_GetLinearVelocity(ballBody);

	if (std::abs(v.x) < 0.01f) {
		v.x *= 100.0f;
	} else if (std::abs(v.y) < 0.01f) {
		v.y *= 100.0f;
	}

	bool speedModify = false;
	if (config::classic) {
		speedModify = ballSpeedClassic(v);
	} else {
		speedModify = ballSpeed(v);
	}
	if (speedModify) {
		b2Body_SetLinearVelocity(ballBody, v);
	}
}

bool Physics::ballSpeed(b2Vec2 &v) {
	float speed = std::sqrt(v.x * v.x + v.y * v.y);
	if (speed > config::speedMax || speed < config::speedMin) {
		v.x *= config::speed / speed;
		v.y *= config::speed / speed;
		return true;
	}
	return false;
}

bool Physics::ballSpeedClassic(b2Vec2 &v) {
	if (v.x < 0.0f) {
		v.x = -config::speedClassic;
	} else if (v.x == 0.0f) {
		v.x = util::randBool() ? config::speedClassic : -config::speedClassic;
	} else {
		v.x = config::speedClassic;
	}
	if (v.y < 0.0f) {
		v.y = -config::speedClassic;
	} else if (v.y == 0.0f) {
		v.y = util::randBool() ? config::speedClassic : -config::speedClassic;
	} else {
		v.y = config::speedClassic;
	}
	return true;
}

b2BodyId Physics::createBall(std::shared_ptr<context::Ball> ball) {

	b2BodyDef ballBodyDef = b2DefaultBodyDef();
	ballBodyDef.type = b2_dynamicBody;
	ballBodyDef.position = ball->pos;
	b2BodyId bb = b2CreateBody(world, &ballBodyDef);

	b2Circle circle = {
		.center = {0.0f, 0.0f},
		.radius = config::ballRadius,
	};
	b2ShapeId ballShape = b2CreateCircleShape(bb, &dsd, &circle);
	b2Shape_SetRestitution(ballShape, 1.0f);

	b2Body_EnableContactEvents(bb, true);

	b2Body_SetBullet(bb, true);
	b2Body_SetLinearVelocity(bb, ball->speed);

	// spdlog::info("ball speed {} {}", ball->speed.x, ball->speed.y);

	return bb;
}

void Physics::createWall() {

	b2BodyDef bd = b2DefaultBodyDef();
	bd.type = b2_staticBody;
	b2BodyId wall = b2CreateBody(world, &bd);

	float left = 0.0f, right = config::gridWF;
	float top = 0.0f, bottom = config::gridHF;

	b2Segment seg;

	// 上边界
	seg.point1 = b2Vec2{left, top};
	seg.point2 = b2Vec2{right, top};
	b2ShapeId wallTop = b2CreateSegmentShape(wall, &dsd, &seg);
	b2Shape_SetRestitution(wallTop, 1.0f);

	// 下边界
	seg.point1 = b2Vec2{left, bottom};
	seg.point2 = b2Vec2{right, bottom};
	b2ShapeId wallBottom = b2CreateSegmentShape(wall, &dsd, &seg);
	b2Shape_SetRestitution(wallBottom, 1.0f);

	// 左边界
	seg.point1 = b2Vec2{left, bottom};
	seg.point2 = b2Vec2{left, top};
	b2ShapeId wallLeft = b2CreateSegmentShape(wall, &dsd, &seg);
	b2Shape_SetRestitution(wallLeft, 1.0f);

	// 右边界
	seg.point1 = b2Vec2{right, bottom};
	seg.point2 = b2Vec2{right, top};
	b2ShapeId wallRight = b2CreateSegmentShape(wall, &dsd, &seg);
	b2Shape_SetRestitution(wallRight, 1.0f);
}

void Physics::createBrick() {

	b2Polygon box = b2MakeBox(0.5f, 0.5f);

	for (const auto &b : d.misc->brick) {

		b2BodyDef bd = b2DefaultBodyDef();
		bd.position = b2Vec2{b.x + 0.5f, b.y + 0.5f};
		bd.type = b2_staticBody;

		b2BodyId bb = b2CreateBody(world, &bd);

		b2Body_Disable(bb);

		brick.push_back(bb);

		b2Body_SetUserData(bb, (void *)&b);

		b2ShapeId si = b2CreatePolygonShape(bb, &dsd, &box);
		b2Shape_SetRestitution(si, 1.0f);
	}
	// spdlog::info("Physics:: brick done {}", brick.size());
}
