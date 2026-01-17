#pragma once

#include "../game/reg.hpp"
#include "dep.hpp"
#include <SDL3/SDL_render.h>

inline static SDL_Color defaultColorX = {230, 230, 230, 255};

namespace render {

struct base {

	renderDep *d;

	base(renderDep *d) : d(d) {};
	virtual ~base() {
	}

	virtual void init() {};
	virtual void render(const game::Reg &_) {};

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

	bool renderRectOutline(SDL_FRect &rect) {
		d->window.calcCameraOffset(rect);
		return SDL_RenderRect(d->r, &rect);
	};

	bool renderFilledRect(SDL_FRect &rect) {
		d->window.calcCameraOffset(rect);
		return SDL_RenderFillRect(d->r, &rect);
	};

	bool renderUIFilledRect(SDL_FRect &rect) {
		return SDL_RenderFillRect(d->r, &rect);
	};

	// 根据角色位置渲染文字
	void text(std::string text, const SDL_FRect &rect) {

		TextTexture tt = {};
		d->text.gen(d->text.getMono32(), text, defaultColorX, tt);

		float size = 1.0f / 32.0f / 3.0f; // 希望 32px 文字是 1 grid 的 1/3 大小

		SDL_FRect r = {.x = rect.x,
			.y = rect.y - 1.5f,
			.w = tt.w * size,
			.h = tt.h * size};

		d->window.calcCameraOffset(r);
		tt.w = r.w;
		tt.h = r.h;

		d->text.put(tt, r.x, r.y, d->window.camera.gridSize * size);
	};
};
}; // namespace render
