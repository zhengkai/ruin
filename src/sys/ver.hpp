#pragma once

#include "../config.hpp"
#include "build.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <cstring>
#include <google/protobuf/stubs/common.h>
#include <spdlog/spdlog.h>
#ifndef __EMSCRIPTEN__
#include <SDL3_ttf/SDL_ttf.h>
#endif
#ifdef _MSC_VER
#include <windows.h>
#endif

inline static void doShowVer() {
	spdlog::info("SDL        = {}.{}.{}",
		SDL_MAJOR_VERSION,
		SDL_MINOR_VERSION,
		SDL_MICRO_VERSION);
#ifndef __EMSCRIPTEN__
	spdlog::info("SDL ttf    = {}.{}.{}",
		SDL_TTF_MAJOR_VERSION,
		SDL_TTF_MINOR_VERSION,
		SDL_TTF_MICRO_VERSION);
#endif
	spdlog::info("SDL image  = {}.{}.{}",
		SDL_IMAGE_MAJOR_VERSION,
		SDL_IMAGE_MINOR_VERSION,
		SDL_IMAGE_MICRO_VERSION);

	spdlog::info("protobuf   = {}",
		google::protobuf::internal::VersionString(GOOGLE_PROTOBUF_VERSION));

	spdlog::info("git        = {}({}{})",
		GIT_BRANCH,
		GIT_HASH,
		(strcmp(GIT_DIRTY, "yes") == 0) ? "-dirty" : "");
	spdlog::info("build time = {}", BUILD_TIMESTAMP);

	std::string s;
	for (const auto &p : config::dirList) {
		if (s != "") {
			s += ", ";
		}
		s += p.string();
	}
	spdlog::info("dir        = {}", s);

#ifdef _MSC_VER
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif
}
