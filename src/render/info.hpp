#pragma once

#include "../build.h"
#include "base.hpp"

namespace render {

struct Info : base {

	using base::base;

	void init() override {};
	void render() override {
		std::string counter = std::to_string(d->window.global.serial);
		d->text.rMono32(counter, d->window.w - 16.0f, 16, Text::Align::RIGHT);

		d->text.rMono32(
			BUILD_TIMESTAMP, 16, d->window.h - 48.0f, Text::Align::LEFT);
	};
};

}; // namespace render
