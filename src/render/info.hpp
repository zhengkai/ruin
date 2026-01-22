#pragma once

#include "../build.h"
#include "../game/reg.hpp"
#include "base.hpp"

namespace render {

struct Info : base {

	using base::base;

	void init() override {};
	void render(const game::Reg &_) override {

		std::string counter = std::to_string(d.window.global.serial);

		auto win = camera.getWinSize();

		d.text.rMono32(counter, win.w - 16.0f, 16, Text::Align::RIGHT);
		d.text.rMono32(BUILD_TIMESTAMP, 16, win.h - 48.0f, Text::Align::LEFT);
		d.text.rMono32(std::to_string(camera.getGridSize()),
			16,
			win.h - 96.0f,
			Text::Align::LEFT);
	};
};

}; // namespace render
