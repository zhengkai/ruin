#pragma once

#include "../asset/asset.hpp"
#include "../config.hpp"
#include "../context/scene.hpp"
#include "body.hpp"
#include "tile.hpp"
#include "util.hpp"
#include "world.hpp"
#include <spdlog/spdlog.h>

namespace physics {

struct Dep {
	context::Scene &scene;
	const asset::Asset &asset;
};

class Physics {

public:
	World world;

private:
	std::size_t serial = 0;

public:
	Physics() {};
	~Physics() {};

	void step() {
		spdlog::info("physics tick");

		for (auto &[_, b] : world.body) {

			spdlog::info(
				"body, x:{}, y:{}, eo.d: {}, vx: {}, vy: {}, serial: {}",
				b.x,
				b.y,
				b.eventOverall.d,
				b.vx,
				b.vy,
				b.serial);

			stepOne(b);
		}
	};

	void stepOne(Body &b) {

		if (b.eventOverall.d == -1.0f) {
			b.vy -= b.vy > 0.0f ? config::gravityUp : config::gravity;
			b.y += b.vy;
		}
		b.x += b.vx;

		for (int t = 3; t >= 0; t--) {
			auto el = CheckIntersects(b, world.tile);
			if (!el.size()) {
				b.eventOverall.Reset();
				b.event.clear();
				continue;
			}

			int i = 0;
			for (auto &eo : el) {
				i++;
				spdlog::info("event {} {} {} {} {} {} {} {}",
					i,
					eo.u,
					eo.d,
					eo.l,
					eo.r,
					eo.hit,
					eo.touch,
					eo.overlap);
			}

			auto eo = IntersectsOverall(el);

			if (!eo.overlap) {
				spdlog::info("no overlap {} {} {}", b.x, b.y, t);
				b.eventOverall = eo;
				b.event = el;
				break;
			}
			if (eo.d > 0.0f && eo.u == -1.0f) {
				spdlog::info("fall down {} {} {}", b.y, eo.d, b.y + eo.d);
				b.y += eo.d;
				b.vy = 0.0f;
			} else if (eo.u > 0.0f && eo.d == -1.0f) {
				spdlog::info("fall up {}", eo.u);
				b.y -= eo.u;
			}
			if (eo.l > 0.0f && eo.r == -1.0f) {
				b.vx = 0.0f;
				b.x += eo.l;
			} else if (eo.r > 0.0f && eo.l == -1.0f) {
				b.vx = 0.0f;
				b.x -= eo.r;
			}
		}
	};

	void clean() {};

	void dump() {
		spdlog::info("Physics tile count: {}", world.tile.size());
	};

	int addBody(float x, float y, float w, float h) {
		auto serial = genSerial();
		auto b = newBody(serial, x, y, w, h);
		world.body[serial] = b;
		return serial;
	};

	int addTile(float x, float y) {
		auto serial = genSerial();
		auto t = newTile(serial, x, y);
		world.tile[serial] = t;
		return serial;
	};

	Body &getBody(std::size_t serial) {
		return world.body.at(serial);
	}

private:
	std::size_t genSerial() {
		return ++serial;
	};
};

}; // namespace physics
