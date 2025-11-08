#pragma once

#include "pb/pose.pb.h"

namespace asset {

inline const std::string poseName(::pb::Pose_Type t) {
	return pb::Pose::Type_descriptor()->FindValueByNumber(t)->name();
};

}; // namespace asset
