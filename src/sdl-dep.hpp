#pragma once

#include "context/ball.h"
#include "context/entity.h"
#include "context/window.h"

struct sdlDep {
	std::shared_ptr<context::BallCluster> ballCluster;
	std::shared_ptr<context::Entity> entity;
	std::shared_ptr<context::Window> window;
};
