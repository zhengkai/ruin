#pragma once

#include <SDL3/SDL_events.h>
#include <optional>

namespace game {

struct Event {
	bool toggleFullscreen = false;
	std::optional<SDL_WindowEvent> resize = {};
	std::optional<SDL_WindowEvent> pixel = {};
	std::optional<SDL_WindowEvent> scale = {};
};

}; // namespace game
