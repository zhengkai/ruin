#pragma once

#include <fmt/format.h>

namespace physics {

enum class MovementState {
	Ground,
	Fall,
	Wall,
	Dashing,
};

enum class Direction {
	None,
	Up,
	Down,
	Left,
	Right,
};

}; // namespace physics

template <>
struct fmt::formatter<physics::Direction> : fmt::formatter<std::string_view> {
	auto format(physics::Direction d, fmt::format_context &ctx) const {
		std::string_view name = "Unknown";
		switch (d) {
		case physics::Direction::None:
			name = "None";
			break;
		case physics::Direction::Up:
			name = "Up";
			break;
		case physics::Direction::Down:
			name = "Down";
			break;
		case physics::Direction::Left:
			name = "Left";
			break;
		case physics::Direction::Right:
			name = "Right";
			break;
		}
		return fmt::formatter<std::string_view>::format(name, ctx);
	}
};
