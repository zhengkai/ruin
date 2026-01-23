#include "sdl.h"
#include "asset/common.hpp"
#include "asset/loader.hpp"
#include "config.hpp"
#include "context/window.hpp"
#include "game/reg.hpp"
#include "render/base.hpp"
#include "render/debug.hpp"
#include "render/dep.hpp"
#include "render/gamepad.hpp"
#include "render/info.hpp"
#include "render/map.hpp"
#include "render/sprite.hpp"
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

sdl::sdl(const context::Window &cw_,
	const asset::Asset &asset_,
	SDL_Renderer *r,
	SDL_Window *w)
	: cw(cw_), asset(asset_), r(r), w(w), rd(text, asset_, r, cw_) {
}

inline asset::Size initWinSize() {

	SDL_DisplayID display = SDL_GetPrimaryDisplay();
	if (!display) {
		return {};
	}
	auto mode = SDL_GetCurrentDisplayMode(display);
	if (!mode) {
		return {};
	}

	spdlog::info("desktop get size {}x{}", mode->w, mode->h);

	asset::Size size = {
		std::floor(static_cast<float>(mode->w) * 0.8f),
		std::floor(static_cast<float>(mode->h) * 0.8f),
	};
	return size;
}

bool sdl::init() {

	if (config::fullscreen) {
		SDL_SetWindowFullscreen(w, true);
	}

	SDL_SetRenderDrawColor(r, 64, 64, 64, 255);
	SDL_RenderClear(r);

	SDL_RenderPresent(r);

	// int drawableWidth, drawableHeight;
	// SDL_GetCurrentRenderOutputSize(r, &drawableWidth, &drawableHeight);
	// spdlog::error("output size {} {}", drawableWidth, drawableHeight);

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

	addRender<render::Debug>();
	addRender<render::Map>();
	addRender<render::Sprite>();
	addRender<render::Gamepad>();
	addRender<render::Info>();
	addRender<render::TerrainChain>();
	for (auto &ren : renderList) {
		ren->init();
	}
}

void sdl::render(const game::Reg &reg) {

	if (!cw.focus) {
		return;
	}

	if (toggleFullscreen()) {
		return;
	}

	auto c = config::colorBg;
	SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
	SDL_RenderClear(r);

	for (auto &ren : renderList) {
		ren->render(reg);
	}

	SDL_RenderPresent(r);
}

bool sdl::toggleFullscreen() {
	if (!cw.event.fullscreeen) {
		return false;
	}
	spdlog::info("toggling fullscreen");
	config::fullscreen = !config::fullscreen;
	SDL_SetWindowFullscreen(w, config::fullscreen);
	return true;
}

sdl::~sdl() {
	if (renderList.size() > 0) {
		renderList.clear();
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

std::unique_ptr<sdl> createSDL(context::Window &cw, asset::Asset &asset) {

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
		SDL_Fail();
		return nullptr;
	}

	auto size = initWinSize();

	cw.camera.setWinSize(size.w, size.h);

	SDL_PropertiesID props = SDL_CreateProperties();

	SDL_SetStringProperty(
		props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, config::winTitle);
	SDL_SetNumberProperty(props,
		SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER,
		static_cast<Sint64>(size.w));
	SDL_SetNumberProperty(props,
		SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER,
		static_cast<Sint64>(size.h));
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

	auto a = asset::Loader(asset, r, config::assetDir);
	if (!a.load()) {
		spdlog::error("Failed to load assets");
		return nullptr;
	}

	return std::make_unique<sdl>(cw, asset, r, w);
}
