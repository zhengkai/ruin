#pragma once

#include "../asset/asset.hpp"
#include "../context/scene.hpp"
#include "../context/window.hpp"
#include "../text.hpp"
#include <SDL3/SDL_render.h>

namespace render {

struct renderDep {
	const Text &text;
	context::Scene &scene;
	context::Window &window;
	const asset::Asset &asset;
	SDL_Renderer *r;

	renderDep(const Text &text,
		const asset::Asset &a,
		SDL_Renderer *r,
		context::Scene &cs,
		context::Window &cw)
		: text(text), scene(cs), window(cw), asset(a), r(r) {};
};

}; // namespace render
