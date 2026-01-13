#include "sdl.h"
#include "asset/loader.hpp"
#include "config.hpp"
#include "context/scene.hpp"
#include "context/window.hpp"
#include "game/reg.hpp"
#include "game/tag.hpp"
#include "render/base.hpp"
#include "render/debug.hpp"
#include "render/dep.hpp"
#include "render/gamepad.hpp"
#include "render/info.hpp"
#include "render/map.hpp"
#include "render/player.hpp"
#include "render/terrain-chain.hpp"
#include "text.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <memory>
#include <spdlog/spdlog.h>

static SDL_AppResult SDL_Fail() {
	SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
	return SDL_APP_FAILURE;
}

sdl::sdl(context::Scene &cs,
	context::Window &cw,
	asset::Asset &asset,
	SDL_Renderer *r,
	SDL_Window *w)
	: scene(cs), window(cw), asset(asset), r(r), w(w) {
}

inline void initWinSize(context::Window &cw) {

	SDL_DisplayID display = SDL_GetPrimaryDisplay();
	if (!display) {
		return;
	}
	auto mode = SDL_GetCurrentDisplayMode(display);
	if (!mode) {
		return;
	}

	spdlog::info("desktop get size {}x{}", mode->w, mode->h);

	cw.setSize(std::floor(static_cast<float>(mode->w) * 0.8f),
		std::floor(static_cast<float>(mode->h) * 0.8f));
}

bool sdl::init() {

	if (config::fullscreen) {
		SDL_SetWindowFullscreen(w, true);
	}

	auto a = asset::Loader(asset, r, config::assetDir);
	if (!a.load()) {
		spdlog::error("Failed to load assets");
		return false;
	}

	SDL_SetRenderDrawColor(r, 64, 64, 64, 255);
	SDL_RenderClear(r);

	SDL_RenderPresent(r);

	// int drawableWidth, drawableHeight;
	// SDL_GetCurrentRenderOutputSize(r, &drawableWidth, &drawableHeight);
	// spdlog::error("output size {} {}", drawableWidth, drawableHeight);

	calcGrid(window.w, window.h);

	if (text.init(r)) {
		spdlog::trace("text inited");
	} else {
		spdlog::error("Failed to init text");
		return false;
	}

	initRender();
	return true;
}

void sdl::initRender() {

	rd = new render::renderDep(text, asset, r, scene, window);

	addRender<render::Map>();
	addRender<render::Player>();
	addRender<render::Debug>();
	addRender<render::Gamepad>();
	addRender<render::Info>();
	addRender<render::TerrainChain>();
	for (auto &ren : renderList) {
		ren->init();
	}
}

void sdl::render(const game::Reg &reg) {

	if (toggleFullscreen()) {
		return;
	}
	renderResize();

	auto view = reg.view<physics::Rect, game::TagPlayer>();
	if (auto entity = view.front(); entity != entt::null) {
		auto &rect = view.get<physics::Rect>(entity);
		window.setFocus(rect.x, rect.y);
	}

	auto c = config::colorBg;
	SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
	SDL_RenderClear(r);

	for (auto &ren : renderList) {
		ren->render(reg);
	}

	SDL_RenderPresent(r);
}

void sdl::renderResize() {
	auto &wr = window.winResize;
	if (!wr.trigger) {
		return;
	}
	spdlog::info("sdl::renderResize called");
	wr.trigger = false;
	calcGrid(wr.w, wr.h);
}

bool sdl::toggleFullscreen() {
	if (!window.toggleFullscreen) {
		return false;
	}
	window.toggleFullscreen = false;
	config::fullscreen = !config::fullscreen;
	SDL_SetWindowFullscreen(w, config::fullscreen);
	return true;
}

void sdl::calcGrid(float wf, float hf) {

	float scale = SDL_GetWindowDisplayScale(w);
	spdlog::info("start sdl::calcGrid, window display scale {:.1f}", scale);

#ifndef _MSC_VER
	wf *= scale;
	hf *= scale;
#endif

	window.calcGrid(wf, hf);
}

sdl::~sdl() {
	if (rd) {
		delete rd;
		rd = nullptr;
	}
	if (renderList.size() > 0) {
		renderList.clear();
	}

	if (ballTex) {
		SDL_DestroyTexture(ballTex);
		ballTex = nullptr;
	}
	if (r) {
		SDL_DestroyRenderer(r);
		r = nullptr;
	}
	if (w) {
		SDL_DestroyWindow(w);
		w = nullptr;
	}
	SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);
	SDL_GL_UnloadLibrary();
	SDL_Quit();
}

std::unique_ptr<sdl> createSDL(
	context::Scene &cs, context::Window &cw, asset::Asset &asset) {

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
		SDL_Fail();
		return nullptr;
	}

	initWinSize(cw);

	SDL_PropertiesID props = SDL_CreateProperties();

	SDL_SetStringProperty(
		props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, config::winTitle);
	SDL_SetNumberProperty(
		props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, static_cast<float>(cw.w));
	SDL_SetNumberProperty(
		props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, static_cast<float>(cw.h));
	SDL_SetNumberProperty(props,
		SDL_PROP_WINDOW_CREATE_FLAGS_NUMBER,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);

#ifdef __EMSCRIPTEN__
	// SDL_SetStringProperty(
	// props, SDL_PROP_WINDOW_CREATE_EMSCRIPTEN_CANVAS_ID_STRING, "#ruin");
#endif

	auto w = SDL_CreateWindowWithProperties(props);

	SDL_DestroyProperties(props);

	if (!w) {
		SDL_Fail();
		return nullptr;
	}

	auto r = SDL_CreateRenderer(w, NULL);
	if (!r) {
		SDL_Fail();
		return nullptr;
	}

	return std::make_unique<sdl>(cs, cw, asset, r, w);
}
