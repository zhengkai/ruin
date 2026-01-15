#pragma once

#include "pb/pose.pb.h"

struct Pose {
	pb::Pose_Type type = pb::Pose_Type::Pose_Type_idle;
	enum class Facing : uint8_t {
		Left,
		Right,
	} facing = Facing::Right;
	int serial = 0;
	int step = 0;
	bool lastRight = false;

	const std::string &typeName() const {
		return pb::Pose_Type_Name(type);
	}

	void change(pb::Pose_Type po) {
		if (type != po) {
			step = 0;
			serial = -1;
		}
		type = po;
	};

	bool isAttack() {
		return type == ::pb::Pose_Type::Pose_Type_attack ||
			type == ::pb::Pose_Type::Pose_Type_attack2 ||
			type == ::pb::Pose_Type::Pose_Type_attack3;
	};

	void parseFacing(const float &x) {
		if (isAttack()) {
			return;
		}
		bool right = lastRight;
		if (x > 0.0f) {
			right = true;
		} else if (x < 0.0f) {
			right = false;
		}
		if ((facing == Pose::Facing::Right) != right) {
			lastRight = right;
			facing = right ? Pose::Facing::Right : Pose::Facing::Left;
		}
	}
};
