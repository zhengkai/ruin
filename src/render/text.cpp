#include "text.h"
#include "../util/path.hpp"
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#ifndef __EMSCRIPTEN__
#include <SDL3_ttf/SDL_ttf.h>
#endif

Text::Text() {
}

bool Text::init(SDL_Renderer *r) {

#ifndef __EMSCRIPTEN__
	this->r = r;

	if (!TTF_Init()) {
		error("TTF_Init fail: {}");
		return false;
	}

	fMono32 = TTF_OpenFont(util::file("JetBrainsMono-Regular.otf").c_str(), 32);
	if (!fMono32) {
		error("init fMono32 fail: {}");
		return false;
	}

	fMono96 = TTF_OpenFont(util::file("JetBrainsMono-Regular.otf").c_str(), 96);
	if (!fMono96) {
		error("init fMono32 fail: {}");
		return false;
	}
#endif

	return true;
}

void Text::rMono32(std::string text, int x, int y, Align align) {
#ifndef __EMSCRIPTEN__
	render(fMono32, text, x, y, align, 32.0f);
#endif
}

void Text::rMono96(std::string text, int x, int y, Align align) {
#ifndef __EMSCRIPTEN__
	render(fMono96, text, x, y, align, 96.0f);
#endif
}

#ifndef __EMSCRIPTEN__
void Text::render(
	TTF_Font *font, std::string text, int x, int y, Align align, float size) {

	SDL_Color color = {230, 230, 230, 255};

	SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), 0, color);
	if (!surface) {
		error("Failed to create text surface, {}");
		return;
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(r, surface);
	SDL_DestroySurface(surface);
	if (!texture) {
		error("Failed to create text texture, %s");
		return;
	}

	SDL_PropertiesID pid = SDL_GetTextureProperties(texture);
	float textWidth =
		(float)SDL_GetNumberProperty(pid, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0);
	float textHeight =
		(float)SDL_GetNumberProperty(pid, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0);

	float ax = static_cast<float>(x);
	float ay = static_cast<float>(y);
	switch (align) {
	case Align::CENTER:
		ax -= textWidth / 2.0f;
		break;
	case Align::RIGHT:
		ax -= textWidth;
		break;
	default:
		break;
	}

	SDL_FRect bgRect = {
		ax - (size / 4.0f), ay, textWidth + (size / 2.0f), textHeight};

	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 128);
	SDL_RenderFillRect(r, &bgRect);

	SDL_FRect dstRect = {ax, ay, textWidth, textHeight};

	SDL_RenderTexture(r, texture, nullptr, &dstRect);
	SDL_DestroyTexture(texture);
}
#endif

void Text::error(std::string_view msg) {
	spdlog::error("{}: {}", msg, SDL_GetError());
}

Text::~Text() {
	r = nullptr;
}
