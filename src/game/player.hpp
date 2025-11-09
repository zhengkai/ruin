#pragma once

#include "../asset/asset.hpp"
#include "../common/control.hpp"
#include "../common/pose.hpp"
#include "../context/entity.hpp"
#include "../context/scene.hpp"
#include "../util/pose.hpp"
#include <pb/pose.pb.h>
#include <spdlog/spdlog.h>

class Player {

private:
	context::Scene &scene;
	const asset::Asset &asset;

public:
	int serial = 0;
	Pose pose = {};
	bool lastRight = true;

private:
	void next() {

		auto dur = scene.player.asset->sprite.at(pose.type)->duration;

		auto frameLimit = dur[pose.step];

		serial++;
		if (serial > frameLimit) {
			serial = 0;
			pose.step++;
			if (static_cast<size_t>(pose.step) >= dur.size()) {
				pose.step = 0;
				if (util::poseIsAttack(pose.type)) {
					pose.type = pb::Pose_Type::Pose_Type_idle;
				}
			}
		}
	}

public:
	Player(context::Scene &c, const asset::Asset &asset)
		: scene(c), asset(asset) {
		spdlog::info("player {}", scene.player.id);
	};
	~Player() {};

	void parse(const Control &control) {

		scene.player.asset = asset.character.at("samurai");

		next();

		// util::poseIsAttack();

		parseFacing(control);
		parseAttack(control);
	}

	void parseAttack(const Control &control) {
		if (!control.btnA) {
			return;
		}
		if (!util::poseIsAttack(pose.type)) {
			pose.type = pb::Pose_Type::Pose_Type_attack1;
			pose.step = 0;
		}
	}

	void parseFacing(const Control &control) {
		if (util::poseIsAttack(pose.type)) {
			return;
		}
		bool right = lastRight;
		if (control.axisA.x > 0.0f) {
			right = true;
		} else if (control.axisA.x < 0.0f) {
			right = false;
		}
		if ((pose.facing == Pose::Facing::Right) != right) {
			lastRight = right;
			pose.facing = right ? Pose::Facing::Right : Pose::Facing::Left;
		}
	}
};
