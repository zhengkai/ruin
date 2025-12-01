#pragma once

#include "asset/asset.hpp"
#include "config.hpp"
#include "context/scene.hpp"
#include <box2d/box2d.h>

inline static b2ShapeDef dsd = [] {
	b2ShapeDef sd = b2DefaultShapeDef();
	sd.material = b2DefaultSurfaceMaterial();
	sd.material.friction = 0.0f;
	sd.density = 1.0f;
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
		if (p.control.x) {
			auto vel = b2Body_GetLinearVelocity(b2p);
			vel.x = p.control.x * p.speed;
			b2Body_SetLinearVelocity(b2p, vel);
		}

		p.setPos(pos);

		// spdlog::info("player pos {} {}", d.scene.player.x, d.scene.player.y);
		// spdlog::info(
		// "terrain size {} {}", terrain.size(), d.asset.map.cell.size());
	};
	void init() {
		initMap();
		initPlayer();
	};
	void initPlayer() {
		b2BodyDef def = b2DefaultBodyDef();
		def.type = b2_dynamicBody;
		def.position = d.scene.player.getPos();
		b2p = b2CreateBody(b2w, &def);

		b2Polygon box = b2MakeBox(0.5f, 0.5f);
		b2ShapeId shape = b2CreatePolygonShape(b2p, &dsd, &box);
		b2Shape_SetRestitution(shape, 0.0f);

		b2Body_SetBullet(b2p, true);
	}
	void initMap() {

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
};

inline std::unique_ptr<World> createWorld(
	context::Scene &cs, asset::Asset &asset) {
	worldDep d(cs, asset);
	return std::make_unique<World>(std::move(d));
}
