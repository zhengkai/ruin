#pragma once

#include "dep.hpp"
#include <SDL3/SDL_render.h>

namespace render {

struct base {

	renderDep *d;

	base(renderDep *d) : d(d) {};
	virtual ~base() {
	}

	virtual void init() {};
	virtual void render() {};

	bool renderTexture(SDL_Texture *t, SDL_FRect &dst) {

		d->window.calcCameraOffset(dst);
		return SDL_RenderTexture(d->r, t, nullptr, &dst);
	};
	bool renderTextureFlipX(SDL_Texture *t, SDL_FRect &dst) {

		SDL_FlipMode flip = SDL_FLIP_HORIZONTAL;
		d->window.calcCameraOffset(dst);
		return SDL_RenderTextureRotated(
			d->r, t, nullptr, &dst, 0.0, nullptr, flip);
	};
	bool renderFillRect(SDL_FRect &rect) {

		d->window.calcCameraOffset(rect);
		return SDL_RenderFillRect(d->r, &rect);
	};
	bool renderUIFillRect(SDL_FRect &rect) {
		return SDL_RenderFillRect(d->r, &rect);
	};
	void text(std::string text) {
		d->text.rMono32(text, 16, d->window.h / 2.0f, Text::Align::LEFT);
	};
};
}; // namespace render
