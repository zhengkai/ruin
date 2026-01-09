#pragma once

#include "../pb/monster.pb.h"
#include "sprite.hpp"

namespace asset {

struct Monster {
	std::string name;
	pb::MonsterDef::Type type;
	std::shared_ptr<SpriteBox> sprite;
	float w;
	float h;
};

}; // namespace asset
