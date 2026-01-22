#pragma once

#include "pb/asset.pb.h"
#include <SDL3_image/SDL_image.h>

namespace asset {

struct Tileset {
	pb::Tileset_Name name = pb::Tileset_Name_unknown;
	std::vector<SDL_Texture *> list = {};

	Tileset(const pb::Tileset_Name &name_) : name{name_} {};

	~Tileset() {
		for (auto texture : list) {
			SDL_DestroyTexture(texture);
		}
		list.clear();
	};
};

}; // namespace asset
