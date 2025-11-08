#pragma once

#include "../common/pose.hpp"
#include "dep.hpp"
#include <SDL3/SDL_render.h>

namespace render {

struct base {

	renderDep *d;

	std::unordered_map<Pose::Type, std::vector<SDL_Texture *>> texMap = {};

	base(renderDep *d) : d(d) {};
	virtual ~base() {
		for (auto &[_, texVec] : texMap) {
			for (auto &tex : texVec) {
				if (tex) {
					SDL_DestroyTexture(tex);
				}
			}
		}
	}

	virtual void init() {};
	virtual void render() {};
};
}; // namespace render
