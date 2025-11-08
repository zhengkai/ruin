#pragma once

#include "../common/pose.hpp"
#include "../sdl-dep.hpp"
#include <SDL3/SDL_render.h>

namespace render {

struct base {

	SDL_Renderer *r;
	sdlDep &d;

	std::unordered_map<Pose::Type, std::vector<SDL_Texture *>> texMap = {};

	base(SDL_Renderer *r, sdlDep &d) : r(r), d(d) {};
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
