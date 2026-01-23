#pragma once

#include "../build.h"
#include "../game/reg.hpp"
#include "base.hpp"
#include <format>

namespace render {

struct Info : base {

	using base::base;

	void init() override {};
	void render(const game::Reg &_) override {

		std::string counter = std::to_string(d.window.global.serial);

		auto win = camera.getWinPixelSize();
		d.text.rMono32(counter, win.w - 16.0f, 16, Text::Align::RIGHT);

		std::vector<std::string> info = {
			std::format("build time: {}", BUILD_TIMESTAMP),
		};

		auto size = camera.getWinSize();
		info.push_back(
			std::format("win size: {}x{}, pixel size: {}x{}, scale: {}",
				size.w,
				size.h,
				win.w,
				win.h,
				camera.getScale()));

		auto cameraRect = camera.focusRect();
		info.push_back(std::format("focus : x={:.2f} y={:.2f} ({:.2f}x{:.2f})",
			cameraRect.x,
			cameraRect.y,
			cameraRect.w,
			cameraRect.h,
			camera.getGridSize()));

		auto [zoom, zoomRate, gridSize] = camera.getZoom();
		info.push_back(std::format("zoom : int {}, rate {:.2f}, grid size "
								   "{:.2f}, grid count {:.1f} x {:.1f}",
			zoom,
			zoomRate,
			gridSize,
			win.w / gridSize,
			win.h / gridSize));

		float y = win.h;
		for (auto s : info) {
			y -= 48.0f;
			d.text.rMono32(s, 16.0f, y, Text::Align::LEFT);
		}
	};
}; // namespace render
}; // namespace render
