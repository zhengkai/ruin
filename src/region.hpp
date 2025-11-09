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
	Region(context::Misc &cm, std::shared_ptr<context::BallGroup> bg)
		: id(bg->region), bg(bg) {

		phy = std::make_unique<Physics>(cm, bg);
	}
	~Region() {
	}
	void update(float dt) {
		phy->update(dt);
	}
};
