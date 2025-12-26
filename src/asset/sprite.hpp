#pragma once

#include "pb/pose.pb.h"
#include <SDL3_image/SDL_image.h>

namespace asset {

struct Sprite {
	pb::Pose::Type pose;
	std::vector<SDL_Texture *> list = {};
	std::vector<int> duration = {};

	~Sprite() {
		for (auto texture : list) {
			SDL_DestroyTexture(texture);
		}
		list.clear();
		duration.clear();
	}
};

}; // namespace asset
