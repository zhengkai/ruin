#pragma once

#include "../asset/sprite.hpp"
#include "../common/pose.hpp"

namespace util {

inline void animation(Pose &pose, std::shared_ptr<asset::SpriteBox> box) {

	auto &dur = box->sprite.at(pose.type)->duration;
	auto frameLimit = dur[pose.step];

	pose.serial++;
	spdlog::info("pose.serial {}", pose.serial);
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
}; // namespace util
