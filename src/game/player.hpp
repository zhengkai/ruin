#pragma once

#include "../asset/asset.hpp"
#include "../common/pose.hpp"
#include "../config.hpp"
#include "../context/entity.hpp"
#include "../context/scene.hpp"
#include "../context/window.hpp"
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
	int serial = 0;
	Pose pose = {};
	bool lastRight = true;
	b2Vec2 prevPos = {};

public:
	Player(const context::Global &global,
		const context::Control &control,
		context::Scene &scene,
		const asset::Asset &asset)
		: global(global), control(control), scene(scene), asset(asset),
		  p(scene.player) {

		spdlog::info("scene {}", this->scene.player.id);

		p.asset = this->asset.character.at("samurai");
		prevPos = p.getPos();
	};
	~Player() {};

	void parse() {

		p.command = {};

		p.command.x = control.axisA.x * p.speed * config::deltaTime;

		next();

		// util::poseIsAttack();

		parseFacing(control.axisA.x);
		if (control.btnX) {
			parseAttack();
		}
		if (control.btnA) {
			parseJump();
		}
		prevPos = p.getPos();
	}

private:
	void next() {

		auto &dur = p.asset->sprite.at(pose.type)->duration;

		auto frameLimit = dur[pose.step];

		serial++;
		if (serial > frameLimit) {
			serial = 0;
			pose.step++;
			if (static_cast<size_t>(pose.step) >= dur.size()) {
				pose.step = 0;
				if (util::poseIsAttack(pose.type)) {
					changePose(pb::Pose_Type::Pose_Type_idle);
				}
			}
		}

		p.prevSpeed.x = p.x - prevPos.x;
		p.prevSpeed.y = p.y - prevPos.y;

		if (prevPos.y == p.y) {
			if (pose.type == pb::Pose_Type::Pose_Type_jump) {
				changePose(pb::Pose_Type::Pose_Type_idle);
			}
		} else {
			if ((p.prevSpeed.y > 0.001f || p.prevSpeed.y < -0.001f) &&
				pose.type != pb::Pose_Type::Pose_Type_jump &&
				!util::poseIsAttack(pose.type)) {

				changePose(pb::Pose_Type::Pose_Type_jump);
			}
		}
		if (pose.type == pb::Pose_Type::Pose_Type_idle && control.axisA.x) {
			changePose(pb::Pose_Type::Pose_Type_run);
		}
		if (pose.type == pb::Pose_Type::Pose_Type_run && !control.axisA.x) {
			changePose(pb::Pose_Type::Pose_Type_idle);
		}
	}

	void changePose(pb::Pose_Type p) {
		if (pose.type != p) {
			pose.step = 0;
		}
		pose.type = p;
	}

	void parseAttack() {
		if (!util::poseIsAttack(pose.type)) {
			pose.type = pb::Pose_Type::Pose_Type_attack1;
			pose.step = 0;
		}
	}

	void parseJump() {

		if (pose.type != pb::Pose_Type::Pose_Type_idle &&
			pose.type != pb::Pose_Type::Pose_Type_walk &&
			pose.type != pb::Pose_Type::Pose_Type_run) {
			return;
		}

		if (!global.counter(jumpCnt, config::cdJump)) {
			return;
		}
		p.command.jump = true;
		changePose(pb::Pose_Type::Pose_Type_jump);
	}

	void parseFacing(const float &x) {
		if (util::poseIsAttack(pose.type)) {
			return;
		}
		bool right = lastRight;
		if (x > 0.0f) {
			right = true;
		} else if (x < 0.0f) {
			right = false;
		}
		if ((pose.facing == Pose::Facing::Right) != right) {
			lastRight = right;
			pose.facing = right ? Pose::Facing::Right : Pose::Facing::Left;
		}
	}
};
