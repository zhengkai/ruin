#pragma once

#include "asset/asset.hpp"
#include "config.hpp"
#include "context/scene.hpp"
#include "map/island.hpp"
#include "util/matrix.hpp"
#include <box2d/box2d.h>

inline static b2ShapeDef dsd = [] {
	b2ShapeDef sd = b2DefaultShapeDef();
	sd.material = b2DefaultSurfaceMaterial();
	sd.material.friction = 0.8f;
	return sd;
}();

struct worldDep {
	context::Scene &scene;
	const asset::Asset &asset;

	worldDep(context::Scene &cs, const asset::Asset &a) : scene(cs), asset(a) {
	}
};

class World {

private:
	b2WorldId b2w;
	worldDep d;

	b2BodyId b2p;
	std::vector<b2BodyId> terrain;

public:
	World(worldDep &&dep) : d(std::move(dep)) {
		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = b2Vec2{0.0f, -9.8f};
		b2w = b2CreateWorld(&worldDef);
		init();
	};
	~World() {
		b2DestroyWorld(b2w);
	}
	void step() {
		_step(config::fpsDeltaTime);
	};

private:
	void _step(float deltaTime) {

		b2World_Step(b2w, deltaTime, 1);
		auto pos = b2Body_GetPosition(b2p);
		if (pos.x < 0.0f || pos.y < 0.0f) {
			pos.x = 10.0f;
			pos.y = 13.0f;
			b2Body_SetLinearVelocity(b2p, b2Vec2{0.0f, 0.0f});
			auto prev = b2Body_GetTransform(b2p);
			prev.p = pos;
			b2Body_SetTransform(b2p, prev.p, prev.q);
		}

		auto &p = d.scene.player;

		// 左右移动
		auto vel = b2Body_GetLinearVelocity(b2p);
		vel.x = p.command.x;
		b2Body_SetLinearVelocity(b2p, vel);

		if (p.command.jump) {
			float mass = b2Body_GetMass(b2p);
			b2Vec2 impulse = {0.0f, mass * 10.0f};
			b2Body_ApplyLinearImpulse(b2p, impulse, b2Vec2_zero, true);
		}

		p.setPos(pos);
		p.command = {};

		// spdlog::info("player pos {} {}", d.scene.player.x,
		// d.scene.player.y); spdlog::info( "terrain size {} {}",
		// terrain.size(), d.asset.map.cell.size());
	};

	void init() {
		initMap();
		initPlayer();
	};

	void initPlayer() {
		b2BodyDef def = b2DefaultBodyDef();
		def.type = b2_dynamicBody;
		def.fixedRotation = true;
		auto &p = d.scene.player;
		def.position = p.getPos();
		b2p = b2CreateBody(b2w, &def);
		b2Body_SetBullet(b2p, true);

		b2Polygon box = b2MakeBox(p.w / 2.0f, p.h / 2.0f);
		b2ShapeId shape = b2CreatePolygonShape(b2p, &dsd, &box);
		b2Shape_SetRestitution(shape, 0.0f);

		b2Body_SetBullet(b2p, true);
	};

	void initMap2() {
		std::vector<b2Vec2> points = {
			{1.0f, 4.5f}, {15.0f, 4.5f}, {15.0f, 5.5f}, {1.0f, 5.5f}};

		b2ChainDef chainDef = b2DefaultChainDef();
		chainDef.points = points.data();
		chainDef.count = static_cast<int>(points.size());
		chainDef.isLoop = true;

		b2BodyDef bd = b2DefaultBodyDef();
		bd.position = b2Vec2{0.0f, 0.0f};
		bd.type = b2_staticBody;

		b2BodyId bb = b2CreateBody(b2w, &bd);
		b2CreateChain(bb, &chainDef);
	};

	void initMap() {

		mapChain();

		b2Polygon box = b2MakeBox(0.5f, 0.5f);
		for (const auto &b : d.asset.map.cell) {

			b2BodyDef bd = b2DefaultBodyDef();
			bd.position = b2Vec2{b.x, b.y};
			bd.type = b2_staticBody;

			b2BodyId bb = b2CreateBody(b2w, &bd);
			terrain.push_back(bb);
			b2Body_SetUserData(bb, (void *)&b);

			b2ShapeId si = b2CreatePolygonShape(bb, &dsd, &box);
			b2Shape_SetRestitution(si, 0.0f);
		}

		if (!terrain.size()) {
			spdlog::warn("terrain size 0, asset cell size: {}",
				terrain.size(),
				d.asset.map.cell.size());
		}
	};

	void mapChain() {
		auto &map = d.asset.map;
		auto m = util::Matrix<uint8_t>(map.w, map.h, 0);
		for (const auto &c : map.cell) {
			int x = static_cast<int>(c.x);
			int y = static_cast<int>(c.y);
			m[x][y] = c.tileName > 0 ? 1 : 0;
		};
		m.dumpASCII();

		::map::Island(m);
	};
};

inline std::unique_ptr<World> createWorld(
	context::Scene &cs, asset::Asset &asset) {
	worldDep d(cs, asset);
	return std::make_unique<World>(std::move(d));
};
