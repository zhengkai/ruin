#pragma once

#include <SDL3/SDL_render.h>

namespace asset {

struct Texture {
	SDL_Texture *texture = nullptr;
	float w;
	float h;
};
}; // namespace asset
