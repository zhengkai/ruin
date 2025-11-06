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

struct Player {
	float x = 0;
	float y = 0;
	Pose pose = {};
};

struct Monster {
	float x = 0;
	float y = 0;
	Pose pose = {};
};

struct Character {
	Player player = {};
	std::vector<Pose> poses = {};
};

}; // namespace context
