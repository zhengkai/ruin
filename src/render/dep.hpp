#pragma once

#include "../asset/asset.hpp"
#include "../context/ball.h"
#include "../context/misc.h"
#include "../context/scene.hpp"
#include <SDL3/SDL_render.h>

namespace render {

struct sdlDep {
	std::shared_ptr<context::BallCluster> ballCluster;
	std::shared_ptr<context::Misc> misc;
};

struct renderDep : sdlDep {
	context::Scene &scene;
	asset::Asset &a;
	SDL_Renderer *r;
	renderDep(
		const sdlDep &d, asset::Asset &a, SDL_Renderer *r, context::Scene &cs)
		: sdlDep(d), a(a), r(r), scene(cs) {};
};

}; // namespace render
