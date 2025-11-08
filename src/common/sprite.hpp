#pragma once

#include "pose.hpp"

struct Sprite {
	Pose::Type type;
	std::vector<SDL_Texture *> frame = {};
	std::vector<int> step = {};
	int size = 0;

	~Sprite() {
		for (auto &f : frame) {
			if (f) {
				SDL_DestroyTexture(f);
				f = nullptr;
			}
		}
		frame.clear();
		step.clear();
	}
};
