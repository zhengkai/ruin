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
	int id = 0;
	int z = 0;
	Pose pose = {};
	physics::Pos prevSpeed = {};

	virtual ~Entity() = default;
};

struct Character : Entity {
	std::shared_ptr<asset::SpriteBox> asset;
	CharacterCommand command = {};
	float speed = 5.0f;
	std::size_t physicsSerial = 0;

	void animation() {

		auto &dur = asset->sprite.at(pose.type)->duration;
		auto frameLimit = dur[pose.step];

		pose.serial++;
		if (pose.serial > frameLimit) {
			pose.serial = 0;
			pose.step++;
			if (static_cast<size_t>(pose.step) >= dur.size()) {
				pose.step = 0;
				if (util::poseIsAttack(pose.type)) {
					pose.change(pb::Pose_Type::Pose_Type_idle);
				}
			}
		}
	}
};

struct Player : Character {};

struct Monster : Character {};

}; // namespace context
