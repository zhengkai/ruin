#pragma once

#include "../common/pose.hpp"
#include "../pb/pose.pb.h"
#include "../physics/body.hpp"
#include "../physics/rect.hpp"
#include "../tag.hpp"
#include "../util/rand.hpp"

namespace decision {

struct Decision {

	const asset::Map &map;

	float speed = 0.03f;

	bool faceRight = false;
	bool run = false;

	Decision(const asset::Map &map_) : map(map_) {
		faceRight = util::randBool();
	};

	void Do(const physics::Rect &rect,
		const tag::PrevPos &prevPos,
		physics::Body &body,
		Pose &pose) {

		if (!body.touch.d) {
			// 脚不着地就停止思考
			return;
		}

		walk(rect, body, pose);

		pose.facing = faceRight ? Pose::Facing::Right : Pose::Facing::Left;

		// spdlog::info("decision {} {}, move {}", rect.x, rect.y, body.vx);
	};

	void walk(const physics::Rect &rect, physics::Body &body, Pose &pose) {

		pose.change(pb::Pose_Type_walk);
		if (body.touch.l && body.touch.r) {
			pose.change(pb::Pose_Type_idle);
			return;
		} else if (body.touch.l) {
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
