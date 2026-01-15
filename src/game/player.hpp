#pragma once

#include "../context/window.hpp"
#include "../physics/body.hpp"
#include "reg.hpp"

namespace game {

inline void playerJump(Reg &reg,
	entt::entity &e,
	const context::Control &control,
	physics::Body &body) {

	body.vx = control.axisA.x * body.speed;

	// 触地一切重置
	if (body.touch.d) {
		if (control.btnA) {
			body.vy = config::jumpForce;
			reg.emplace<tag::Jump>(e, true, 1);
		} else {
			reg.remove<tag::Jump>(e);
		}
		return;
	}

	auto jp = reg.tryGet<tag::Jump>(e);
	if (control.btnA) {
		if (jp) {
			// 二段跳
			if (!jp->isJumping && jp->cnt < config::maxJumpCount) {
				body.vy = config::jumpForce;
				jp->cnt++;
			}
		} else {
			// 掉落时跳
			body.vy = config::jumpForce;
			reg.emplace<tag::Jump>(e, true, 2);
		}
	} else {
		if (jp) {
			// 小跳
			jp->isJumping = false;
			if (body.vy > config::jumpShortForce) {
				body.vy *= config::jumpShortForce;
			}
		}
	}
};

}; // namespace game
