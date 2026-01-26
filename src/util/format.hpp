#pragma once

#include "../pb/asset.pb.h"
#include "../pb/pose.pb.h"
#include <fmt/format.h>

template <>
struct fmt::formatter<pb::Tileset_Name> : fmt::formatter<std::string_view> {
	template <typename FormatContext>
	auto format(pb::Tileset_Name v, FormatContext &ctx) const {
		return fmt::formatter<std::string_view>::format(
			pb::Tileset_Name_Name(v), ctx);
	}
};

template <>
struct fmt::formatter<pb::Pose_Type> : fmt::formatter<std::string_view> {
	template <typename FormatContext>
	auto format(pb::Pose_Type v, FormatContext &ctx) const {
		return fmt::formatter<std::string_view>::format(
			pb::Pose_Type_Name(v), ctx);
	}
};
