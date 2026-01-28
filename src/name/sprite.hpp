#pragma once

#include "../pb/sprite.pb.h"
#include <fmt/format.h>

namespace name {

struct [[nodiscard]] Sprite {

private:
	pb::SpriteBox_Name v;

public:
	Sprite(pb::SpriteBox_Name name_) : v(name_) {};

	constexpr pb::SpriteBox_Name value() const noexcept {
		return v;
	}

	constexpr explicit operator bool() const noexcept {
		return v != pb::SpriteBox_Name{0};
	}

	friend constexpr bool operator==(Sprite lhs, Sprite rhs) noexcept {
		return lhs.v == rhs.v;
	}

	friend constexpr bool operator!=(Sprite lhs, Sprite rhs) noexcept {
		return lhs.v != rhs.v;
	}
};

const auto SpriteNone = name::Sprite(pb::SpriteBox_Name{0});
}; // namespace name

namespace std {
template <> struct hash<name::Sprite> {
	size_t operator()(const name::Sprite &z) const noexcept {
		return std::hash<int>{}(static_cast<int>(z.value()));
	}
};
} // namespace std

template <>
struct fmt::formatter<name::Sprite> : fmt::formatter<std::string_view> {
	template <typename FormatContext>
	auto format(name::Sprite v, FormatContext &ctx) const {
		return fmt::formatter<std::string_view>::format(
			pb::SpriteBox_Name_Name(v.value()), ctx);
	}
};
