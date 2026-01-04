#pragma once

#include "pb/pose.pb.h"

namespace util {

inline const std::string poseName(::pb::Pose_Type t) {
	return pb::Pose::Type_descriptor()->FindValueByNumber(t)->name();
};

inline bool poseIsAttack(::pb::Pose_Type t) {
	return t == ::pb::Pose_Type::Pose_Type_attack ||
		t == ::pb::Pose_Type::Pose_Type_attack2 ||
		t == ::pb::Pose_Type::Pose_Type_attack3;
};

}; // namespace util
