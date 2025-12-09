#pragma once

#include "../asset/asset.hpp"
#include "../context/misc.h"
#include "../context/scene.hpp"
#include "../context/window.hpp"
#include "../text.hpp"
#include <SDL3/SDL_render.h>

namespace render {

struct renderDep {
	const Text &text;
	const context::Misc &misc;
	context::Scene &scene;
	context::Window &window;
	const asset::Asset &asset;
	SDL_Renderer *r;

	renderDep(const Text &text,
		const asset::Asset &a,
		SDL_Renderer *r,
		context::Scene &cs,
		const context::Misc &cm,
		context::Window &cw)
		: text(text), misc(cm), scene(cs), window(cw), asset(a), r(r) {};
};

}; // namespace render
