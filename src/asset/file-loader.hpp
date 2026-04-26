#pragma once

#include "texture.hpp"
#include <SDL3_image/SDL_image.h>
#include <filesystem>
#include <spdlog/spdlog.h>

namespace asset {

struct FileLoader {
	SDL_Renderer *r;
	std::filesystem::path dir;

	FileLoader(SDL_Renderer *r_, std::filesystem::path dir_)
		: r{r_}, dir{dir_} {};

	bool load(const std::string &path, Texture &t) {
		auto file = dir / path;

#ifdef _MSC_VER
		auto u8 = file.generic_u8string();	 // std::u8string
		std::string p(u8.begin(), u8.end()); // char8_t -> char (逐字节拷贝)
		SDL_Surface *s = IMG_Load(p.c_str());
#else
		SDL_Surface *s = IMG_Load(file.c_str());
#endif

		if (!s) {
			spdlog::error("load image fail: {}", path);
			return false;
		}

		if (s->w <= 0 || s->h <= 0) {
			spdlog::error("invalid image size: {} ({}x{})", path, s->w, s->h);
			SDL_DestroySurface(s);
			return false;
		}

		t.w = static_cast<float>(s->w);
		t.h = static_cast<float>(s->h);
		t.texture = SDL_CreateTextureFromSurface(r, s);
		SDL_DestroySurface(s);

		return true;
	}
};

}; // namespace asset
