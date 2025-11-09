#include "sdl.h"
#include "asset/init.hpp"
#include "config.hpp"
#include "context/scene.hpp"
#include "context/window.h"
#include "render/base.hpp"
#include "render/dep.hpp"
#include "render/player.hpp"
#include "render/text.h"
#include "util/path.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <memory>
#include <spdlog/spdlog.h>

static SDL_AppResult SDL_Fail() {
	SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
	return SDL_APP_FAILURE;
}

sdl::sdl(context::BallCluster &cb,
	context::Scene &cs,
	context::Window &cw,
	context::Misc &cm,
	asset::Asset &asset,
	SDL_Renderer *r,
	SDL_Window *w)
	: scene(cs), window(cw), misc(cm), asset(asset), cb(cb), r(r), w(w) {
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

	cw.w = static_cast<int>(std::floor(static_cast<float>(mode->w) * 0.8f));
	cw.h = static_cast<int>(std::floor(static_cast<float>(mode->h) * 0.8f));
}

bool sdl::init() {

	if (config::fullscreen) {
		SDL_SetWindowFullscreen(w, true);
	}

	if (!asset::Load(r, config::assetDir, asset)) {
		spdlog::error("Failed to load assets");
		return false;
	}

	rd = new render::renderDep(cb, asset, r, scene, misc);

	SDL_SetRenderDrawColor(r, 64, 64, 64, 255);
	SDL_RenderClear(r);

	SDL_RenderPresent(r);
	auto img = util::file("circle.webp");
	spdlog::info("img {}", img);

	SDL_Surface *surf = IMG_Load(img.c_str());
	if (!surf) {
		spdlog::error("Failed to load ball image");
		SDL_Fail();
		return false;
	}
	ballTex = SDL_CreateTextureFromSurface(r, surf);

	SDL_DestroySurface(surf);

	if (!ballTex) {
		spdlog::error("ball SDL_CreateTextureFromSurface failed");
		SDL_Fail();
		return false;
	}

	// int drawableWidth, drawableHeight;
	// SDL_GetCurrentRenderOutputSize(r, &drawableWidth, &drawableHeight);
	// spdlog::error("output size {} {}", drawableWidth, drawableHeight);

	calcGrid(window.w, window.h);

	text = new Text();
	if (text->init(r)) {
		spdlog::trace("text inited");
	} else {
		spdlog::error("Failed to init text");
		return false;
	}

	initRender();

	return true;
}

void sdl::initRender() {
	renderList.emplace_back(std::make_unique<render::Player>(rd));
	for (auto &ren : renderList) {
		ren->init();
	}
}

void sdl::renderCounter() {
	std::string counter = std::to_string(window.serial);
	text->rMono32(counter, window.w - 16, 16, Text::Align::RIGHT);
}

void sdl::render() {

	if (toggleFullscreen()) {
		return;
	}
	renderResize();

	auto c = config::colorBg;
	SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
	SDL_RenderClear(r);

	renderBrick();

	if (window.showBall) {
		auto gl = cb.group;
		for (auto &bg : gl) {
			for (auto &b : bg->list) {

				auto bc = bg->color;
				if (gl.size() == 2) {
					bc = gl[1 - bg->region]->color;
				} else {
					bc = bc.Lighten(0.5);
				}
				renderBall(b, bc.ToColor());
			}
		}
	}

	renderCounter();
	renderControlMsg();

	for (auto &ren : renderList) {
		ren->render();
	}

	// renderGamepad();

	SDL_RenderPresent(r);
}

void sdl::renderResize() {
	auto wr = window.winResize;
	if (wr == nullptr) {
		return;
	}
	window.winResize = nullptr;
	calcGrid(wr->w, wr->h);
	delete wr;
}

void sdl::renderBrick() {

	SDL_FRect rect;

	auto rl = misc.brick;

	std::vector<SDL_FRect> edges;

	calcRegionSize();

	bool cw = false, cn = false, ce = false, cs = false;

	for (const auto &b : rl) {
		rect.w = this->window.gridSize;
		rect.h = this->window.gridSize;
		rect.x = this->window.startX + b.x * this->window.gridSize;
		rect.y = this->window.startY + b.y * this->window.gridSize;

		SDL_FRect corner;
		corner.w = config::brickBorder;
		corner.h = config::brickBorder;
		if (b.region < 0) {
			if (config::brickBorder <= 0.0f) {
				continue;
			}
			corner.x = rect.x - config::brickBorder;
			corner.y = rect.y - config::brickBorder;
			edges.push_back(corner);

			corner.x = rect.x + rect.w;
			corner.y = rect.y - config::brickBorder;
			edges.push_back(corner);

			corner.x = rect.x + rect.w;
			corner.y = rect.y + rect.h;
			edges.push_back(corner);

			corner.x = rect.x - config::brickBorder;
			corner.y = rect.y + rect.h;
			edges.push_back(corner);
			continue;
		}

		if (config::brickBorder > 0.0f) {
			cw = false;
			cn = false;
			ce = false;
			cs = false;
			auto s = b.side;
			if (s.e < 0 || rl[s.e].region != b.region) {
				ce = true;
				rect.w -= config::brickBorder;
			}
			if (s.s < 0 || rl[s.s].region != b.region) {
				cs = true;
				rect.h -= config::brickBorder;
			}
			if (s.w < 0 || rl[s.w].region != b.region) {
				cw = true;
				rect.x += config::brickBorder;
				rect.w -= config::brickBorder;
			}
			if (s.n < 0 || rl[s.n].region != b.region) {
				cn = true;
				rect.y += config::brickBorder;
				rect.h -= config::brickBorder;
			}
		}

		auto group = cb.group[b.region];
		auto bc = group->color;

		// double power = b.power * 2.0f;
		double power = b.power;

		auto g = group->gradation;
		power = ((b.x - group->size.x) / group->size.w * g.w) *
			((b.y - group->size.y) / group->size.h * g.h);

		// spdlog::info(
		// 	"power={:.2f} bx={} wf={} gx={} gw={} x-rate={:.2f} x-final={:.2f}",
		// 	power,
		// 	b.x,
		// 	config::gridWF,
		// 	group->size.x,
		// 	group->size.w,
		// 	(b.x - group->size.x) / group->size.w,
		// 	(b.x - group->size.x) / group->size.w * g.w);

		SDL_Color c = bc.Lighten(power).ToColor();

		SDL_SetRenderDrawColor(r, c.r, c.g, c.b, 255);
		SDL_RenderFillRect(r, &rect);

		if (config::brickBorder > 0.0f) {
			c = config::colorBg;
			SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);

			if (cw && cn) {
				corner.x = rect.x - config::brickBorder * 2.0f;
				corner.y = rect.y - config::brickBorder * 2.0f;
				SDL_RenderFillRect(r, &corner);
			}
			if (cn && ce) {
				corner.x = rect.x + rect.w + config::brickBorder;
				corner.y = rect.y - config::brickBorder * 2.0f;
				SDL_RenderFillRect(r, &corner);
			}
			if (ce && cs) {
				corner.x = rect.x + rect.w + config::brickBorder;
				corner.y = rect.y + rect.h + config::brickBorder;
				edges.push_back(corner);
			}
			if (cs && cw) {
				corner.x = rect.x - config::brickBorder * 2.0f;
				corner.y = rect.y + rect.h + config::brickBorder;
				edges.push_back(corner);
			}
		}
	}

	if (config::brickBorder > 0.0f) {
		auto c = config::colorBg;
		SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
		for (const auto &e : edges) {
			SDL_RenderFillRect(r, &e);
		}
	}
}

void sdl::calcRegionSize() {
	struct RegionSize {
		int w;
		int e;
		int n;
		int s;
	};
	std::vector<RegionSize> li;
	auto gl = cb.group;
	li.reserve(gl.size());
	for (size_t i = 0; i < gl.size(); i++) {
		li.push_back(
			RegionSize{.w = config::gridW, .e = 0, .n = config::gridH, .s = 0});
	}

	for (const auto &b : misc.brick) {
		if (b.region < 0) {
			continue;
		}
		auto g = gl[b.region];
		auto &r = li[b.region];
		if (r.w > b.x) {
			r.w = b.x;
		}
		if (r.e < b.x) {
			r.e = b.x;
		}
		if (r.n > b.y) {
			r.n = b.y;
		}
		if (r.s < b.y) {
			r.s = b.y;
		}
		g->size.x = static_cast<float>(r.w);
		g->size.w = std::max(3.0f, static_cast<float>(r.e) - g->size.x);
		g->size.y = static_cast<float>(r.n);
		g->size.h = std::max(3.0f, static_cast<float>(r.s) - g->size.y);
	}
}

void sdl::renderBall(std::shared_ptr<context::Ball> b, SDL_Color c) {

	SDL_FRect rect;
	rect.x = window.startX + (b->pos.x - config::ballRadius) * window.gridSize;
	rect.y = window.startY + (b->pos.y - config::ballRadius) * window.gridSize;
	rect.w = window.gridSize * config::ballRadius / 0.5f;
	rect.h = rect.w;

	// spdlog::trace("ball = {} {} {}", rect.x, rect.y, rect.w);

	SDL_SetTextureColorMod(ballTex, c.r, c.g, c.b);

	SDL_RenderTexture(r, ballTex, nullptr, &rect);
}

void sdl::renderControlMsg() {
	context::ControlMsg *c = window.controlMsg;
	if (c == nullptr) {
		return;
	}
	if (c->expireSerial < window.serial) {
		delete c;
		window.controlMsg = nullptr;
		return;
	}
	text->rMono96(c->msg, window.w / 2, window.h - 192, Text::Align::CENTER);
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

void sdl::calcGrid(int winW, int winH) {

	float scale = SDL_GetWindowDisplayScale(w);
	spdlog::info("start sdl::calcGrid, window display scale {:.1f}", scale);

	float ww = static_cast<float>(winW);
	float wh = static_cast<float>(winH);
#ifndef _MSC_VER
	ww *= scale;
	wh *= scale;
#endif

	window.w = static_cast<int>(ww);
	window.h = static_cast<int>(wh);

	float wf = config::gridWF;
	float hf = config::gridHF + cfgPaddingTop;

	float gs = std::floor(ww / wf < wh / hf ? ww / wf : wh / hf);

	window.gridSize = gs;
	spdlog::info("gridSize = {}, win = {}x{}", gs, winW, winH);

	window.startX = std::floor((ww - (gs * wf)) / 2);
	window.startY = std::floor((wh - (gs * hf)) / 2 + (gs * cfgPaddingTop));
	spdlog::info(
		"grid {}x{}={}, grid pixel: {}x{}, win pixel: {}x{}, start: x={},y={}",
		config::gridW,
		config::gridH,
		config::gridW * config::gridH,
		gs * config::gridW,
		gs * config::gridH,
		window.w,
		window.h,
		window.startX,
		window.startY);
}

void sdl::renderGamepad() {

	std::string x = std::to_string(misc.gamepadX);
	std::string y = std::to_string(misc.gamepadY);

	text->rMono32(x, 512, 512, Text::Align::RIGHT);
	text->rMono32(y, 512, 554, Text::Align::RIGHT);
}

sdl::~sdl() {
	if (text) {
		delete text;
		text = nullptr;
	}

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

std::unique_ptr<sdl> createSDL(context::BallCluster &cb,
	context::Scene &cs,
	context::Window &cw,
	context::Misc &cm,
	asset::Asset &asset) {

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
		SDL_Fail();
		return nullptr;
	}

	initWinSize(cw);

	SDL_PropertiesID props = SDL_CreateProperties();

	SDL_SetStringProperty(
		props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, config::winTitle);
	SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, cw.w);
	SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, cw.h);
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

	return std::make_unique<sdl>(cb, cs, cw, cm, asset, r, w);
}
