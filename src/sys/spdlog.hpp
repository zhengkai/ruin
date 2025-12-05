#pragma once

#include "../config.hpp"
#ifndef __EMSCRIPTEN__
#include <spdlog/async.h>
#include <spdlog/async_logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#endif
#ifdef _MSC_VER
#include <windows.h>
#endif

inline void spdlogInit() {

#ifdef _MSC_VER
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif

#ifndef __EMSCRIPTEN__
	spdlog::init_thread_pool(8192, 1);

	auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

	spdlog::async_logger logger("async_stdout",
		{stdout_sink},
		spdlog::thread_pool(),
		spdlog::async_overflow_policy::block);

	spdlog::register_logger(std::make_shared<spdlog::async_logger>(logger));

	spdlog::stdout_color_mt("console");
#endif

	// 默认日志级别 info
	spdlog::level::level_enum log_level = spdlog::level::info;

	if (config::verbose == "trace") {
		log_level = spdlog::level::trace;
	} else if (config::verbose == "debug") {
		log_level = spdlog::level::debug;
	} else if (config::verbose == "warn") {
		log_level = spdlog::level::warn;
	} else if (config::verbose == "err") {
		log_level = spdlog::level::err;
	} else if (config::verbose == "critical") {
		log_level = spdlog::level::critical;
	} else if (config::verbose == "off") {
		log_level = spdlog::level::off;
	}

	spdlog::set_level(log_level);
	spdlog::set_pattern("%m-%d %H:%M:%S.%e %^[%l]%$ %v");
}
