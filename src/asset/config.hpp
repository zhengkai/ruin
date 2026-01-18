#pragma once

#include "map.hpp"
#include <string>

namespace asset {

struct Config {
	std::string playerSprite = "samurai";
	MapTarget zoneStart;
};

}; // namespace asset
