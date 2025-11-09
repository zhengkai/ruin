#pragma once

#include "context/scene.hpp"
#include "context/window.h"
#include "render/base.hpp"
#include "render/dep.hpp"
#include "render/rainbow.hpp"
#include "render/text.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <box2d/box2d.h>
#include <memory>

class sdl {

private:
	context::Scene &scene;
	context::Window &window;
	context::Misc &misc;
	context::BallCluster &cb;

	render::renderDep *rd = nullptr;
	SDL_Renderer *r = nullptr;
	Text *text = nullptr;
	// SDL_Texture *bg;
	SDL_Window *w = nullptr;
	SDL_Texture *ballTex = nullptr;
	std::unique_ptr<Rainbow> rainbow;
	std::vector<std::unique_ptr<render::base>> renderList;

public:
	sdl(context::BallCluster &cb,
		context::Scene &cs,
		context::Window &cw,
		context::Misc &cm);
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

sdl *createSDL();
