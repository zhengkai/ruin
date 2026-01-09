#pragma once

#include "../pb/monster.pb.h"
#include "sprite.hpp"

namespace asset {

struct Monster {
	pb::MonsterDef::Type type;
	std::shared_ptr<SpriteBox> sprite;
};

}; // namespace asset
