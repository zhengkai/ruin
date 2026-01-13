#pragma once

#include "./pb/monster.pb.h"
#include <cstddef>

namespace tag {

struct Player {
	std::size_t idx = 0;
};

struct Monster {
	pb::MonsterDef::Type type;
};

struct Tile {};

}; // namespace tag
