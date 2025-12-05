#pragma once

#include "../asset/character.hpp"
#include "../common/pose.hpp"
#include <box2d/box2d.h>

namespace context {

struct CharacterCommand {
	float x = 0.0f;
	bool jump = 0;
};

struct Entity {
	int id = 0;
	float x = 0.0f;
	float y = 0.0f;
	float w = 1.0f;
	float h = 1.0f;
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
	SDL_FRect getRect() {
		return SDL_FRect{.x = x, .y = y, .w = w, .h = h};
	}
};

struct Character : Entity {
	std::shared_ptr<asset::Character> asset;
	CharacterCommand command = {};
	float speed = 2.0f;
};
struct Player : Character {
	Player() {
		w = 0.75f;
		h = 1.5f;
	}
};
struct Enemy : Character {};

}; // namespace context
