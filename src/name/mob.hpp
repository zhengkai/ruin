#pragma once

#include "../pb/mob.pb.h"
#include <fmt/format.h>

namespace name {

struct [[nodiscard]] MobType {

private:
	pb::Mob_Type v;

public:
	MobType(pb::Mob_Type type_) : v{type_} {};

	constexpr pb::Mob_Type value() const noexcept {
		return v;
	}

	constexpr explicit operator bool() const noexcept {
		return v != pb::Mob_Type{0};
	}

	friend constexpr bool operator==(MobType lhs, MobType rhs) noexcept {
		return lhs.v == rhs.v;
	}

	friend constexpr bool operator!=(MobType lhs, MobType rhs) noexcept {
		return lhs.v != rhs.v;
	}
};

const auto MobNone = name::MobType(pb::Mob_Type{0});
}; // namespace name

namespace std {
template <> struct hash<name::MobType> {
	size_t operator()(const name::MobType &z) const noexcept {
		return std::hash<int>{}(static_cast<int>(z.value()));
	}
};
} // namespace std

template <>
struct fmt::formatter<name::MobType> : fmt::formatter<std::string_view> {
	template <typename FormatContext>
	auto format(name::MobType v, FormatContext &ctx) const {
		return fmt::formatter<std::string_view>::format(
			pb::Mob_Type_Name(v.value()), ctx);
	}
};
