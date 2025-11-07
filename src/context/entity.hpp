#pragma once

#include "../common/def.hpp"

namespace context {

struct Entity {
	int id = 0;
	float x = 0;
	float y = 0;
	int z = 0;
	Pose pose = {};
	virtual ~Entity() = default;
};

struct Character : Entity {};

struct Player : Character {};
struct Enemy : Character {};

}; // namespace context
