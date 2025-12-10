#pragma once

#include "util/path.hpp"
#include <SDL3/SDL_render.h>
#include <spdlog/spdlog.h>
#include <string>
#ifndef __EMSCRIPTEN__
#include <SDL3_ttf/SDL_ttf.h>
#endif

static std::filesystem::path fontFile = "asset/font/JetBrainsMono-Regular.otf";

static SDL_Color defaultColor = {230, 230, 230, 255};

struct TextTexture {
	SDL_Texture *t;
	float w;
	float h;
};

class Text {

private:
	SDL_Renderer *r;
#ifndef __EMSCRIPTEN__
	TTF_Font *fMono32;
	TTF_Font *fMono96;
#endif

public:
	enum class Align {
		LEFT,
		CENTER,
		RIGHT,
	};
	Text() {};
	~Text() {
		r = nullptr;
	};

	TTF_Font *getMono32() const {
		return fMono32;
	}

	bool init(SDL_Renderer *r) {
#ifndef __EMSCRIPTEN__
		this->r = r;

		if (!TTF_Init()) {
			error("TTF_Init fail: {}");
			return false;
		}

		fMono32 = TTF_OpenFont(util::file(fontFile).c_str(), 32);
		if (!fMono32) {
			error("init fMono32 fail: {}");
			return false;
		}

		fMono96 = TTF_OpenFont(util::file(fontFile).c_str(), 96);
		if (!fMono96) {
			error("init fMono32 fail: {}");
			return false;
		}
#endif
		return true;
	};
	void rMono32(std::string text, float x, float y, Align align) const {
#ifndef __EMSCRIPTEN__
		render(fMono32, text, x, y, align, 32.0f);
#endif
	};

	void rMono96(std::string text, float x, float y, Align align) const {
#ifndef __EMSCRIPTEN__
		render(fMono96, text, x, y, align, 96.0f);
#endif
	};

	bool gen(TTF_Font *font,
		std::string text,
		SDL_Color &color,
		TextTexture &tt) const {

		SDL_Surface *surface =
			TTF_RenderText_Solid(font, text.c_str(), 0, color);
		if (!surface) {
			error("Failed to create text surface, {}");
			return false;
		}

		tt.t = SDL_CreateTextureFromSurface(r, surface);
		SDL_DestroySurface(surface);
		if (!tt.t) {
			error("Failed to create text texture, %s");
			return false;
		}

		SDL_PropertiesID pid = SDL_GetTextureProperties(tt.t);
		tt.w =
			(float)SDL_GetNumberProperty(pid, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0);
		tt.h = (float)SDL_GetNumberProperty(
			pid, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0);

		return true;
	};

	void put(TextTexture &tt, float x, float y, float size) const {
		SDL_FRect bgRect = {.x = x - (size / 4.0f),
			.y = y,
			.w = tt.w + (size / 2.0f),
			.h = tt.h};

		SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(r, 0, 0, 0, 128);
		SDL_RenderFillRect(r, &bgRect);

		SDL_FRect dstRect = {.x = x, .y = y, .w = tt.w, .h = tt.h};

		SDL_RenderTexture(r, tt.t, nullptr, &dstRect);
		SDL_DestroyTexture(tt.t);
	};

private:
#ifndef __EMSCRIPTEN__
	void render(TTF_Font *font,
		std::string text,
		float x,
		float y,
		Align align,
		float size) const {

		TextTexture tt = {};
		if (!gen(font, text, defaultColor, tt)) {
			return;
		}

		switch (align) {
		case Align::CENTER:
			x -= tt.w / 2.0f;
			break;
		case Align::RIGHT:
			x -= tt.w;
			break;
		default:
			break;
		}

		put(tt, x, y, size);
	};
#endif
	void error(std::string_view msg) const {
		spdlog::error("{}: {}", msg, SDL_GetError());
	};
};
