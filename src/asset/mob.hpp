#pragma once

#include "../pb/mob.pb.h"
#include "sprite.hpp"

namespace asset {

struct Mob {
	SpriteBox &sprite;
	pb::MobDef_Type type;
	float scale = 0.0f;
};

}; // namespace asset
