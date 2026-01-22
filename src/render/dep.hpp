#pragma once

#include "../asset/asset.hpp"
#include "../context/window.hpp"
#include "../text.hpp"
#include <SDL3/SDL_render.h>

namespace render {

struct renderDep {
	const Text &text;
	const context::Window &window;
	const asset::Asset &asset;
	SDL_Renderer *r;

	renderDep(const Text &text,
		const asset::Asset &a,
		SDL_Renderer *r,
		const context::Window &cw)
		: text(text), window(cw), asset(a), r(r) {};
};

}; // namespace render
