#pragma once

#include "../common/pose.hpp"
#include "../physics/body.hpp"
#include "../physics/rect.hpp"
#include "../tag.hpp"

namespace decision {

struct Decision {

	bool run = false;

	void Do(const physics::Rect &rect,
		const tag::PrevPos &prevPos,
		physics::Body &body,
		Pose &pose) {

		if (!body.touch.d) {
			// 脚不着地就停止思考
			return;
		}

		// spdlog::info("decision {} {}", rect.x, rect.y);
	};
};

}; // namespace decision
