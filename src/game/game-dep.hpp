#pragma once

#include "../context/misc.h"
#include "../context/scene.hpp"
#include "../context/window.h"

struct GameDep {
	std::shared_ptr<context::Misc> misc;
	std::shared_ptr<context::Window> window;
	std::shared_ptr<context::Scene> scene;
};
