#pragma once

#include "../common/pose.hpp"
#include "../context/window.hpp"
#include "../physics/body.hpp"
#include "reg.hpp"

namespace game {

inline bool playerEnterZone(
	Reg &reg, const physics::Rect &rect, context::Window &window) {

	auto view = reg.view<asset::MapGate>();
	for (auto [_, gate] : view.each()) {
		if (rect.isOverlap(gate.rect)) {
			window.enterZone = gate.target;
			return true;
		}
	}

	return false;
};

inline bool playerAttack(Pose &pose) {
	if (pose.isAttack()) {
		return false;
	}
	pose.change(pb::Pose_Type::Pose_Type_attack);
	return true;
};

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

inline void playerPose(const context::Control &control,
	const tag::PrevPos &prevPos,
	const physics::Rect &rect,
	Pose &pose) {

	pose.parseFacing(control.axisA.x);

	using enum pb::Pose_Type;

	float prevSpeedY = rect.y - prevPos.y;
	if (prevSpeedY == 0.0f) {
		if (pose.type == Pose_Type_jump) {
			pose.change(Pose_Type_idle);
		}
	} else if ((prevSpeedY > 0.001f || prevSpeedY < -0.001f) &&
		pose.type != Pose_Type_jump && !pose.isAttack()) {

		pose.change(Pose_Type_jump);
	}

	if (pose.type == Pose_Type_idle && control.axisA.x) {
		pose.change(Pose_Type_run);
	} else if (pose.type == Pose_Type_run && !control.axisA.x) {
		pose.change(Pose_Type_idle);
	}
};

}; // namespace game
