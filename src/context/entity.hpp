#pragma once

#include "../asset/character.hpp"
#include "../common/pose.hpp"
#include "../physics/pos.hpp"
#include "../physics/rect.hpp"
#include <SDL3/SDL_rect.h>

namespace context {

struct CharacterCommand {
	float x = 0.0f;
	bool jump = false;
};

struct Entity : physics::Rect {
	int id = 0;
	int z = 0;
	Pose pose = {};
	physics::Pos prevSpeed = {};

	virtual ~Entity() = default;

	physics::Pos getPos() {
		return *this;
	};

	void setPos(const physics::Pos &pos) {
		x = pos.x;
		y = pos.y;
	};
	SDL_FRect getRect() {
		return SDL_FRect{.x = x, .y = y, .w = w, .h = h};
	};
};

struct Character : Entity {
	std::shared_ptr<asset::Character> asset;
	CharacterCommand command = {};
	float speed = 5.0f;
	std::size_t physicsSerial = 0;
};
struct Player : Character {
	Player() {
		w = 0.75f;
		h = 1.5f;
	}
};
struct Enemy : Character {};

}; // namespace context
