#pragma once

namespace asset {

struct Tileset {
	std::string name = "";
	std::vector<SDL_Texture *> list = {};

	~Tileset() {
		for (auto texture : list) {
			SDL_DestroyTexture(texture);
		}
		list.clear();
	}
};

}; // namespace asset
