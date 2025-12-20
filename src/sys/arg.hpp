#pragma once

#include "../config.hpp"
#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>

inline int parseArg(int argc, char **argv) {
	CLI::App app{"Ruin"};

	app.add_flag("-v,--version", config::versionOnly, "Print version only");

	app.add_flag("-f,--fullscreen", config::fullscreen, "Fullscreen");

	app.add_option("--verbose",
		   config::verbose,
		   "Verbose level (info, debug, trace, warn, err, critical, off)")
		->default_val("info")
		->expected(0, 1)
		->check(CLI::IsMember(
			{"info", "debug", "trace", "warn", "err", "critical", "off"}));

	try {
		app.parse(argc, argv);
	} catch (const CLI::CallForHelp &e) {
		std::cout << app.help() << std::endl;
		return 127; // 表示要退出
	} catch (const CLI::ParseError &e) {
		return 1;
	}

	config::setGrid(config::gridWDefault, config::gridHDefault);

	return 0;
}
