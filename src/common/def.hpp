#pragma once

struct Pose {
	enum class Type {
		Idle,
		Walk,
		Run,
		Jump,
		Attack,
	} type = Type::Idle;
	enum class Facing {
		Left,
		Right,
	} facing = Facing::Right;
	int typeID = 0;
	int step = 0;
};
