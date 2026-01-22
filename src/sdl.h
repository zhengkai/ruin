#pragma once

#include "asset/common.hpp"
#include "context/window.hpp"
#include "render/base.hpp"
#include "render/dep.hpp"
#include "text.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <memory>

class sdl {

private:
	context::Window &cw;
	const asset::Asset &asset;

	render::renderDep *rd = nullptr;
	SDL_Renderer *r = nullptr;
	Text text = {};
	// SDL_Texture *bg;
	SDL_Window *w = nullptr;
	SDL_Texture *ballTex = nullptr;
	std::vector<std::unique_ptr<render::base>> renderList;

public:
	sdl(context::Window &cw,
		const asset::Asset &asset,
		SDL_Renderer *r,
		SDL_Window *w);
	~sdl();
	bool init();
	void handleInput(SDL_Event *e);

	void render(const game::Reg &reg);

private:
	void renderControlMsg();
	void renderResize();
	void calcGrid(float wf, float hf);
	bool toggleFullscreen();
	void initRender();

	template <typename T> void addRender() {
		renderList.emplace_back(std::make_unique<T>(rd));
	}
};

inline asset::Size initWinSize(context::Window &cw);

std::unique_ptr<sdl> createSDL(context::Window &cw, asset::Asset &asset);
