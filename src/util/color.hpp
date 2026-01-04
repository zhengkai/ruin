#pragma once

#include "hct.hpp"
#include <SDL3/SDL_pixels.h>
#include <spdlog/spdlog.h>

namespace util {

inline std::vector<SDL_Color> mapColor = {
	// copy from https://colorbrewer2.org/#type=qualitative&scheme=Paired&n=12
	// thanks the site
	{166, 206, 227, 255},
	{31, 120, 180, 255},
	{178, 223, 138, 255},
	{51, 160, 44, 255},
	{251, 154, 153, 255},
	{227, 26, 28, 255},
	{253, 191, 111, 255},
	{255, 127, 0, 255},
	{202, 178, 214, 255},
	{106, 61, 154, 255},
	{255, 255, 153, 255},
	{177, 89, 40, 255},
};

inline void shuffleMapColor() {
	std::shuffle(mapColor.begin(), mapColor.end(), rng());
}

inline SDL_Color lighten(const SDL_Color sc, float amount = 0.2f) {
	auto hct = util::HCT(sc);
	auto tone = hct.getTone();
	hct.setHue(tone * (1.0f + amount));
	spdlog::info(
		"lighten: {:.02f} {:.02f} {:.02f}", amount, tone, hct.getTone());
	return hct.ToColor();
};

inline SDL_Color invert(const SDL_Color sc) {
	SDL_Color result;
	result.r = static_cast<Uint8>(255 - sc.r);
	result.g = static_cast<Uint8>(255 - sc.g);
	result.b = static_cast<Uint8>(255 - sc.b);
	result.a = sc.a;
	return result;
};

}; // namespace util
