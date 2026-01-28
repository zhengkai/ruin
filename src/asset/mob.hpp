#pragma once

#include "../name/mob.hpp"
#include "sprite.hpp"

namespace asset {

struct Mob {
	name::MobType type;
	SpriteBox &sprite;
};

}; // namespace asset
