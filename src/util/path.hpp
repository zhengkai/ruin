#pragma once

#include "../config.hpp"
#include <filesystem>
#include <string>
#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#elif !defined(__EMSCRIPTEN__)
#include <unistd.h>
#endif

namespace fs = std::filesystem;

namespace util {

inline std::string file(const std::filesystem::path in) {
	for (const auto &dir : config::dirList) {
		auto fullPath = dir / in;
		if (fs::exists(fullPath)) {
			auto u8 = fullPath.u8string();
			std::string s(u8.begin(), u8.end());
#ifdef _MSC_VER
			std::ranges::replace(s, '/', '\\');
#endif
			return s;
		}
	}
	auto s = in.string();
	spdlog::critical("File not found: {}", s);
	return s;
};

inline fs::path getAppDir() {

#ifdef _MSC_VER
	wchar_t buf[MAX_PATH];
	GetModuleFileNameW(nullptr, buf, MAX_PATH);
	return fs::path(buf).parent_path();
#elif defined(__APPLE__)
	char buf[1024];
	uint32_t size = sizeof(buf);
	_NSGetExecutablePath(buf, &size);
	return fs::path(buf).parent_path();
#elif defined(__EMSCRIPTEN__)
	return "";
#else
	char buf[1024];
	ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
	if (len != -1) {
		buf[len] = '\0';
		return fs::path(buf).parent_path();
	}
	return fs::current_path();
#endif
};

inline void InitDir() {
	if (!config::dirList.empty()) {
		return;
	}
#ifdef __EMSCRIPTEN__
	config::dirList.push_back(fs::path("/static"));
	return;
#endif
	auto appDir = getAppDir();

	const char *envDir = std::getenv("RUIN_DIR");
	if (envDir) {
		auto pDir = fs::path(envDir);
		if (pDir != appDir) {
			config::dirList.push_back(pDir);
		}
	}
	config::dirList.push_back(appDir);
};
} // namespace util
