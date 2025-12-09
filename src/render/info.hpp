#pragma once

#include "../build.h"
#include "base.hpp"

namespace render {

struct Info : base {

	using base::base;

	void init() override {};
	void render() override {
		std::string counter = std::to_string(d->window.global.serial);
		d->text.rMono32(counter,
			static_cast<int>(d->window.w) - 16,
			16,
			Text::Align::RIGHT);

		d->text.rMono32(BUILD_TIMESTAMP,
			16,
			static_cast<int>(d->window.h) - 48,
			Text::Align::LEFT);
	};
};

}; // namespace render
