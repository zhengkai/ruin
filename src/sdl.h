#pragma once

#include "render/base.hpp"
#include "render/rainbow.hpp"
#include "render/text.h"
#include "sdl-dep.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <box2d/box2d.h>
#include <memory>

class sdl {

private:
	SDL_Renderer *r;
	Text *text;
	// SDL_Texture *bg;
	SDL_Window *w;
	SDL_Texture *ballTex;
	sdlDep d;
	std::unique_ptr<Rainbow> rainbow;
	std::vector<std::unique_ptr<render::base>> renderList;

public:
	sdl(sdlDep &dep);
	~sdl();
	bool init();
	void handleInput(SDL_Event *e);

	void render();

private:
	void renderControlMsg();
	void renderCounter();
	void renderResize();
	void renderBall(std::shared_ptr<context::Ball> b, SDL_Color c);
	void renderBrick();
	void calcGrid(int winW, int winH);
	void initWinSize();
	void calcRegionSize();
	bool toggleFullscreen();
	void renderGamepad();
	void initRender();
};
