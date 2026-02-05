#pragma once

#include "pb/asset.pb.h"
#include "texture.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <spdlog/spdlog.h>
#include <vector>

namespace asset {

inline std::vector<SDL_Texture *> loadTileset(
	SDL_Renderer *r, Texture &t, pb::TilesetSize &size) {

	SDL_FRect dst{
		0.0f, 0.0f, static_cast<float>(size.w()), static_cast<float>(size.h())};

	std::vector<SDL_Texture *> fl;
	fl.reserve(static_cast<size_t>((t.w / size.w()) * (t.h / size.h())));

	for (float y = 0.0f; y < t.h; y += dst.h) {
		for (float x = 0.0f; x < t.w; x += dst.w) {
			SDL_FRect src{.x = x, .y = y, .w = dst.w, .h = dst.h};

			SDL_Texture *tile = SDL_CreateTexture(r,
				SDL_PIXELFORMAT_RGBA32,
				SDL_TEXTUREACCESS_TARGET,
				static_cast<int>(size.w()),
				static_cast<int>(size.h()));

			SDL_SetRenderTarget(r, tile);
			SDL_RenderTexture(r, t.texture, &src, &dst);
			SDL_SetRenderTarget(r, nullptr);

			fl.push_back(tile);
		}
	}
	SDL_DestroyTexture(t.texture);
	return fl;
};
}; // namespace asset
