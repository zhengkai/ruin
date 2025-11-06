#pragma once

#include "../util/path.hpp"
#include <SDL3/SDL.h>
#include <vector>

namespace util {

std::vector<SDL_Texture *> loadSpriteFrames(
	SDL_Renderer *r, const std::filesystem::path file) {

	auto img = util::file(file);
	SDL_Surface *s = IMG_Load(img.c_str());
	if (!s) {
		spdlog::error("load image fail: {}", img);
		return {};
	}

	int w = s->w;
	int h = s->h;
	if (w == 0 || h == 0) {
		spdlog::error("image has zero width or height: {}", img);
		return {};
	}

	SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
	SDL_DestroySurface(s);

	if (!t) {
		SDL_Log("Failed to create texture: %s", SDL_GetError());
		spdlog::error("create texture fail: {}", img);
		return {};
	}

	int num = w / h;

	float wf = static_cast<float>(w);
	float hf = static_cast<float>(h);

	std::vector<SDL_Texture *> fl;
	SDL_FRect dst{0.0f, 0.0f, hf, hf};
	for (float x = 0.0f; x < wf; x += hf) {
		SDL_FRect src{x, 0, hf, hf};
		SDL_Texture *f = SDL_CreateTexture(
			r, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, h, h);

		SDL_SetRenderTarget(r, f);
		SDL_RenderTexture(r, t, &src, &dst);
		SDL_SetRenderTarget(r, nullptr);
		fl.push_back(f);
	}

	return fl;
};
}; // namespace util
