#pragma once

#include "../name/zone.hpp"
#include "map.hpp"
#include <SDL3/SDL_rect.h>

namespace asset {

struct Zone {
	name::Zone name;
	Map &map;
	std::vector<MapGate> gate;
	std::vector<MapGate> exit;
	std::vector<MapMob> mob;

	Zone(name::Zone name_, Map &m) : name{name_}, map{m} {};
};

}; // namespace asset
