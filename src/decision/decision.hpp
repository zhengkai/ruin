#pragma once

#include "../common/pose.hpp"
#include "../pb/pose.pb.h"
#include "../physics/body.hpp"
#include "../physics/rect.hpp"
#include "../tag.hpp"
#include "../util/rand.hpp"

namespace decision {

enum class ActionMode {
	Idle,
	Walk,
};

struct Decision {

	const asset::Map &map;

	float speed = 0.03f;

	bool faceRight = false;
	bool run = false;
	ActionMode actionMode = ActionMode::Walk;
	std::size_t actionLife = 0;

	Decision(const asset::Map &map_) : map(map_) {
		faceRight = util::randBool();
		speed *= 0.5f + util::randFloat();
	};

	void Do(const physics::Rect &rect,
		const tag::PrevPos &prevPos,
		physics::Body &body,
		Pose &pose) {

		if (!body.touch.d) {
			// 脚不着地就停止思考
			return;
		}

		think();

		if (actionMode == ActionMode::Idle) {
			pose.change(pb::Pose_Type_idle);
			body.vx = 0.0f;
		} else {
			walk(rect, body, pose);
			pose.facing = faceRight ? Pose::Facing::Right : Pose::Facing::Left;
		}

		// spdlog::info("decision {} {}, move {}", rect.x, rect.y, body.vx);
	};

	void think() {
		if (actionLife > 0) {
			actionLife--;
			return;
		}
		actionMode = util::randBool() ? ActionMode::Walk : ActionMode::Idle;
		actionLife = 100 + static_cast<std::size_t>(util::randFloat() * 200.0f);
		faceRight = util::randBool();

		if (actionMode == ActionMode::Idle) {
			actionLife /= 2;
		}
	};

	void walk(const physics::Rect &rect, physics::Body &body, Pose &pose) {

		pose.change(pb::Pose_Type_walk);
		if (body.touch.l) {
			if (body.touch.r) {
				pose.change(pb::Pose_Type_idle);
				return;
			}
			faceRight = true;
		} else if (body.touch.r) {
			faceRight = false;
		}

		body.vx = map.checkMove(rect, speed * (faceRight ? 1.0f : -1.0f));
		if (body.vx == 0.0f) {
			faceRight = !faceRight;
		}
	};
};

}; // namespace decision
