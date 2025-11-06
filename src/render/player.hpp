#pragma once

#include "../util/rand.hpp"
#include "../util/sprite.hpp"
#include "base.hpp"
#include <SDL3_image/SDL_image.h>
#include <spdlog/spdlog.h>

static std::uniform_int_distribution<int> stepDist(0, 7);

namespace render {

struct Player : base {

	using base::base;

	std::vector<SDL_Texture *> walk;

	void init() override {

		walk = util::loadSpriteFrames(r, "assets/Samurai/Walk.png");
		if (walk.empty()) {
			spdlog::error("Failed to load player walk sprites");
			return;
		}

		spdlog::info("Render::Player init done {}", walk.size());
	};
	void render() override {

		float size = 500.0f;

		SDL_FRect dst = {200.0f, 200.0f, size, size};
		SDL_RenderTexture(r, walk[stepDist(util::rng())], nullptr, &dst);
	};
};
}; // namespace render
