#pragma once

#include "../asset/asset.hpp"
#include "../common/pose.hpp"
#include "../config.hpp"
#include "../context/entity.hpp"
#include "../context/scene.hpp"
#include "../context/window.hpp"
#include "../physics/pos.hpp"
#include "../util/pose.hpp"
#include <pb/pose.pb.h>
#include <spdlog/spdlog.h>

class Player {

private:
	const context::Global &global;
	const context::Control &control;
	context::Scene &scene;
	const asset::Asset &asset;
	context::Player &p;

public:
	int jumpCnt = 0;
	bool lastRight = true;
	physics::Pos prevPos = {};

public:
	Player(const context::Global &global,
		const context::Control &control,
		context::Scene &scene,
		const asset::Asset &asset)
		: global(global), control(control), scene(scene), asset(asset),
		  p(scene.player) {

		spdlog::info("scene {}", this->scene.player.id);

		prevPos = p.getPos();
	};
	~Player() {};

	void parse() {

		p.command = {};

		if (control.btnU && enterMap()) {
			return;
		}

		p.command.x = control.axisA.x * p.speed * config::deltaTime;

		next();

		// util::poseIsAttack();

		p.pose.parseFacing(control.axisA.x);
		if (control.btnX) {
			parseAttack();
		}
		if (control.btnA) {
			parseJump();
		}
		prevPos = p.getPos();
	}

	bool enterMap() {
		if (scene.enterMap.name != "") {
			return false;
		}

		for (auto &g : scene.map->gate) {
			if (p.isOverlap(g.rect)) {
				scene.enterMap = g.target;
				return true;
			}
		}

		return false;
	}

private:
	void next() {

		p.prevSpeed.x = p.x - prevPos.x;
		p.prevSpeed.y = p.y - prevPos.y;

		if (prevPos.y == p.y) {
			if (p.pose.type == pb::Pose_Type::Pose_Type_jump) {
				p.pose.change(pb::Pose_Type::Pose_Type_idle);
			}
		} else {
			if ((p.prevSpeed.y > 0.001f || p.prevSpeed.y < -0.001f) &&
				p.pose.type != pb::Pose_Type::Pose_Type_jump &&
				!p.pose.isAttack()) {

				p.pose.change(pb::Pose_Type::Pose_Type_jump);
			}
		}
		if (p.pose.type == pb::Pose_Type::Pose_Type_idle && control.axisA.x) {
			p.pose.change(pb::Pose_Type::Pose_Type_run);
		}
		if (p.pose.type == pb::Pose_Type::Pose_Type_run && !control.axisA.x) {
			p.pose.change(pb::Pose_Type::Pose_Type_idle);
		}
	}

	void parseAttack() {
		if (!p.pose.isAttack()) {
			p.pose.type = pb::Pose_Type::Pose_Type_attack;
			p.pose.step = 0;
		}
	}

	void parseJump() {

		if (p.pose.type != pb::Pose_Type::Pose_Type_idle &&
			p.pose.type != pb::Pose_Type::Pose_Type_walk &&
			p.pose.type != pb::Pose_Type::Pose_Type_run) {
			return;
		}

		if (!global.counter(jumpCnt, config::cdJump)) {
			return;
		}
		p.command.jump = true;
		p.pose.change(pb::Pose_Type::Pose_Type_jump);
	}
};
