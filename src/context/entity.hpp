#pragma once

#include "../asset/character.hpp"
#include "../common/pose.hpp"
#include <box2d/box2d.h>

namespace context {

struct Entity {
	int id = 0;
	float x = 0;
	float y = 0;
	int z = 0;
	Pose pose = {};

	virtual ~Entity() = default;

	b2Vec2 getPos() {
		return b2Vec2{x, y};
	}
	void setPos(const b2Vec2 &pos) {
		x = pos.x;
		y = pos.y;
	}
};

struct Character : Entity {
	std::shared_ptr<asset::Character> asset;
};
struct Player : Character {};
struct Enemy : Character {};

}; // namespace context
