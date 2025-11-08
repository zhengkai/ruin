#pragma once

#include "../asset/asset.hpp"
#include "../context/ball.h"
#include "../context/misc.h"
#include "../context/scene.hpp"
#include "../context/window.h"
#include <SDL3/SDL_render.h>

namespace render {

struct sdlDep {
	std::shared_ptr<context::BallCluster> ballCluster;
	std::shared_ptr<context::Misc> misc;
	std::shared_ptr<context::Window> window;
	std::shared_ptr<context::Scene> scene;
};

struct renderDep : sdlDep {
	asset::Asset &a;
	SDL_Renderer *r;
	renderDep(const sdlDep &d, asset::Asset &a, SDL_Renderer *r)
		: sdlDep(d), a(a), r(r) {};
};

}; // namespace render
