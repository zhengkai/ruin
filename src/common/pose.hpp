#pragma once

struct Pose {
	enum class Type : uint8_t {
		Idle,
		Walk,
		Run,
		Jump,
		Attack,
	} type = Type::Idle;
	enum class Facing : uint8_t {
		Left,
		Right,
	} facing = Facing::Right;
	int typeID = 0;
	int step = 0;
};

namespace std {
template <> struct hash<Pose::Type> {
	size_t operator()(const Pose::Type &t) const noexcept {
		return static_cast<size_t>(t);
	}
};
}; // namespace std
