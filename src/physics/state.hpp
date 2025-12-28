#pragma once

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
