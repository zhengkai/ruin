#pragma once

#include "../pb/map.pb.h"
#include <fmt/format.h>

namespace name {

struct [[nodiscard]] Zone {

private:
	pb::Zone_Name v;

public:
	Zone(pb::Zone_Name name_) : v(name_) {};

	constexpr pb::Zone_Name value() const noexcept {
		return v;
	}

	constexpr explicit operator bool() const noexcept {
		return v != pb::Zone_Name{0};
	}

	friend constexpr bool operator==(Zone lhs, Zone rhs) noexcept {
		return lhs.v == rhs.v;
	}

	friend constexpr bool operator!=(Zone lhs, Zone rhs) noexcept {
		return lhs.v != rhs.v;
	}
};

const auto ZoneNone = name::Zone(pb::Zone_Name{0});
}; // namespace name

namespace std {
template <> struct hash<name::Zone> {
	size_t operator()(const name::Zone &z) const noexcept {
		return std::hash<int>{}(static_cast<int>(z.value()));
	}
};
} // namespace std

template <>
struct fmt::formatter<name::Zone> : fmt::formatter<std::string_view> {
	template <typename FormatContext>
	auto format(name::Zone v, FormatContext &ctx) const {
		return fmt::formatter<std::string_view>::format(
			pb::Zone_Name_Name(v.value()), ctx);
	}
};
