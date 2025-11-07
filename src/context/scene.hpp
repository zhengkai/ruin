#pragma once

#include "entity.hpp"
#include <vector>

namespace context {

struct Scene {
	Player player = {};
	std::vector<Enemy> enemy;
};

}; // namespace context
