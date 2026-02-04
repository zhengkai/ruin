#pragma once

#include "../name/zone.hpp"
#include "map.hpp"
#include "texture.hpp"
#include <SDL3/SDL_rect.h>

namespace asset {

struct ZoneBackground : Texture {
	std::size_t index = 0;

	std::optional<float> loopCnt;

	~ZoneBackground() {
		if (texture != nullptr) {
			SDL_DestroyTexture(texture);
			texture = nullptr;
		}
	};
};

struct Zone {
	name::Zone name;
	Map &map;
	std::vector<MapGate> gate;
	std::vector<MapGate> exit;
	std::vector<MapMob> mob;
	std::vector<ZoneBackground> background;

	Zone(name::Zone name_, Map &m) : name{name_}, map{m} {};
};

}; // namespace asset
