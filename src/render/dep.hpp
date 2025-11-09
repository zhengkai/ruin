#pragma once

#include "../asset/asset.hpp"
#include "../context/ball.h"
#include "../context/misc.h"
#include "../context/scene.hpp"
#include <SDL3/SDL_render.h>

namespace render {

struct renderDep {
	context::Misc &misc;
	context::Scene &scene;
	const asset::Asset &asset;
	SDL_Renderer *r;
	context::BallCluster &cb;

	renderDep(context::BallCluster &cb,
		const asset::Asset &a,
		SDL_Renderer *r,
		context::Scene &cs,
		context::Misc &cm)
		: misc(cm), scene(cs), asset(a), r(r), cb(cb) {};
};

}; // namespace render
