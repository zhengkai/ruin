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

	void change(pb::Pose_Type po) {
		if (type != po) {
			step = 0;
		}
		type = po;
	};

	bool isAttack() {
		return type == ::pb::Pose_Type::Pose_Type_attack ||
			type == ::pb::Pose_Type::Pose_Type_attack2 ||
			type == ::pb::Pose_Type::Pose_Type_attack3;
	};
};
