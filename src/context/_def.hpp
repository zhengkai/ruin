#pragma once

namespace context {

struct Pose {
	enum class Type {
		Idle,
		Walk,
		Run,
		Jump
	} type = Type::Idle;
};

struct Entity {
	int id = 0;
	float x = 0;
	float y = 0;
	int z = 0;
	Pose pose = {};
	virtual ~Entity() = default;
};

}; // namespace context
