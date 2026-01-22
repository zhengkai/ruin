#pragma once

#include "../pb/monster.pb.h"
#include "sprite.hpp"

namespace asset {

struct Monster {
	SpriteBox &sprite;
	pb::MonsterDef_Type type;
	float scale = 0.0f;
};

}; // namespace asset
