#include "asset/init.hpp"
#include "ruin.h"
#include "sys/arg.hpp"
#include "sys/spdlog.hpp"
#include "sys/ver.hpp"
#include "util/path.hpp"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>

static Ruin *theRuin = nullptr;

void wasmLoop() {
	if (theRuin->stop) {
		emscripten_cancel_main_loop();
		return;
	}
	theRuin->loop();
}
#endif

int main(int argc, char *argv[]) {

	util::InitDir();

	int pa = parseArg(argc, argv);
	if (pa) {
		return pa == 127 ? 0 : pa;
	}

	spdlogInit();

	doShowVer();
	if (config::versionOnly) {
		return 0;
	}

	asset::Init();

	Ruin p;
	if (!p.init()) {
		return 1;
	}

#ifdef __EMSCRIPTEN__
	theRuin = &p;
	emscripten_set_main_loop(wasmLoop, 0, true);
#else
	while (!p.stop) {
		p.loop();
	}

	spdlog::get("async_stdout")->flush();
	spdlog::shutdown();
#endif
	return 0;
}
