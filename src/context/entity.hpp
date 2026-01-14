#pragma once

#include "../asset/sprite.hpp"
#include "../common/pose.hpp"
#include "../physics/pos.hpp"
#include <SDL3/SDL_rect.h>

namespace context {

struct CharacterCommand {
	float x = 0.0f;
	bool jump = false;
};

struct Entity {
	Pose pose = {};
	physics::Pos prevSpeed = {};

	virtual ~Entity() = default;
};

struct Character : Entity {
	CharacterCommand command = {};
	float speed = 5.0f;
};

struct Player : Character {};

struct Monster : Character {};

}; // namespace context
