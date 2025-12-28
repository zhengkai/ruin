#pragma once

#include "config.hpp"
#include <chrono>
#include <cmath>
#include <spdlog/spdlog.h>

using namespace std::chrono;

class Time {
public:
	explicit Time() : start(steady_clock::now()) {
		ft = std::chrono::duration<float>(config::deltaTime);
#ifndef __EMSCRIPTEN__
		spdlog::info("time start, {} fps, {:.6f} s per frame",
			config::fps,
			config::deltaTime);
#endif
	}

	// 每帧开始时调用
	void tick() {
		auto now = steady_clock::now();
		auto t0 = duration<float>(now - start);
		auto ti = std::chrono::floor<seconds>(t0);
		auto t = t0 - ti;
		auto idx = static_cast<int>(std::floor(t / ft));
		auto next = (idx + 1) * ft - t;
		spdlog::trace("frame = {} {:.6f}s, cost {:6.3f}ms",
			idx,
			t.count(),
			1000 * (ft - next).count());
		std::this_thread::sleep_for(next);
	}

private:
	duration<float> ft; // 每帧目标时长 (秒)
	steady_clock::time_point start;
};
