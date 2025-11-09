#pragma once

#include "../common/pose.hpp"
#include "dep.hpp"
#include <SDL3/SDL_render.h>

namespace render {

struct base {

	renderDep *d;

	base(renderDep *d) : d(d) {};
	virtual ~base() {
	}

	virtual void init() {};
	virtual void render() {};
};
}; // namespace render
