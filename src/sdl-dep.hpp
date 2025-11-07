#pragma once

#include "context/ball.h"
#include "context/misc.h"
#include "context/scene.hpp"
#include "context/window.h"

struct sdlDep {
	std::shared_ptr<context::BallCluster> ballCluster;
	std::shared_ptr<context::Misc> misc;
	std::shared_ptr<context::Window> window;
	std::shared_ptr<context::Scene> scene;
};
