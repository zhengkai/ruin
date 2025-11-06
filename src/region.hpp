#pragma once

#include "context/misc.h"
#include "physics.h"
#include <spdlog/spdlog.h>

struct Region {

private:
	int id;
	std::unique_ptr<Physics> phy;
	std::shared_ptr<context::BallGroup> bg;

public:
	Region(std::shared_ptr<context::Misc> e,
		std::shared_ptr<context::BallGroup> bg)
		: bg(bg), id(bg->region) {

		phy = std::make_unique<Physics>(
			PhysicsDep{
				.misc = e,
			},
			bg);
	}
	~Region() {
		spdlog::trace("region destory {}", id);
	}
	void update(float dt) {
		phy->update(dt);
	}
};
