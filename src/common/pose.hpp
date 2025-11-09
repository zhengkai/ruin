#pragma once

#include "pb/pose.pb.h"

struct Pose {
	::pb::Pose_Type type = pb::Pose_Type::Pose_Type_idle;
	enum class Facing : uint8_t {
		Left,
		Right,
	} facing = Facing::Right;
	int step = 0;
};
