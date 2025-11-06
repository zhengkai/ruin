#pragma once

#include "../sdl-dep.hpp"
#include <SDL3/SDL_render.h>

namespace render {

struct base {

	SDL_Renderer *r;
	sdlDep &d;

	base(SDL_Renderer *r, sdlDep &d) : r(r), d(d) {};

	virtual void init() {};
	virtual void render() {};
};
}; // namespace render
