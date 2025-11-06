#pragma once

#include "../config.hpp"
#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>

inline int parseArg(int argc, char **argv) {
	CLI::App app{"Ruin"};

	app.add_flag("-v,--version", config::versionOnly, "Print version only");

	app.add_option(
		   "-t,--theme", config::colorTheme, "Color theme (hct, map, rand)")
		->default_val(config::ColorTheme::Map)
		->transform(
			CLI::CheckedTransformer(std::map<std::string, config::ColorTheme>({
										{"hct", config::ColorTheme::HCT},
										{"map", config::ColorTheme::Map},
										{"rand", config::ColorTheme::Rand},
									}),
				CLI::ignore_case));

	app.add_flag("-c,--classic",
		config::classic,
		"Classic mode (like \"vnglst/ruin-wars\")");

	std::vector<int> region;
	app.add_option("-r,--region", region, "Region number")->delimiter(',');

	app.add_flag("-f,--fullscreen", config::fullscreen, "Fullscreen");

	int w;
	app.add_option("-W,--grid-w", w, "Grid width")
		->default_val(config::gridWDefault)
		->check(CLI::Range(10, 100));
	int h;
	app.add_option("-H,--grid-h", h, "Grid height")
		->default_val(config::gridHDefault)
		->check(CLI::Range(10, 100));

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

	int rsize = region.size();
	if (rsize == 0) {
		region = std::vector<int>(5, 1);
	} else if (rsize == 1) {
		rsize = std::max(std::min(region[0], 99), 2);
		region = std::vector<int>(rsize, 1);
	}

	config::setGrid(w, h);
	config::setRegion(region);

	return 0;
}
