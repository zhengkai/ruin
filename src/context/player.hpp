#pragma once

#include "_def.hpp"
#include <vector>

namespace context {

struct Character : Entity {};

struct Player : Character {};
struct Enemy : Character {};

struct CharacterList {
	Player player = {};
	std::vector<Enemy> enemy;
};

}; // namespace context
