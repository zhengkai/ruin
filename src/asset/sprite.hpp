#pragma once

namespace asset {

struct Sprite {
	std::vector<SDL_Texture *> list = {};
	std::vector<int> duration = {};

	~Sprite() {
		for (auto texture : list) {
			SDL_DestroyTexture(texture);
		}
	}
};

}; // namespace asset
