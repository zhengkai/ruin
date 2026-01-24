#pragma once

#include "config.hpp"
#include "context/monitor.hpp"
#include <chrono>
#include <cmath>
#include <spdlog/spdlog.h>

using namespace std::chrono;

class Time {
public:
	explicit Time() : start(steady_clock::now()) {
		ft = duration<double>(config::deltaTime);
#ifndef __EMSCRIPTEN__
		spdlog::info("time start, {} fps, {:.6f} s per frame",
			config::fps,
			config::deltaTime);
#endif
	}

	// 每帧开始时调用
	void tick(context::FrameMonitor &fm) {
		auto now = steady_clock::now();
		auto t0 = duration<double>(now - start);
		auto ti = floor<seconds>(t0);
		auto t = t0 - ti;
		auto idx = static_cast<int>(std::floor(t / ft));
		auto next = (idx + 1) * ft - t;
		float costMS = 1000.0f * (ft - next).count();
		fm.pushTimeUs(static_cast<std::size_t>(costMS * 1000.0f));

		spdlog::trace("frame = {:3d}, {:.6f}s, cost = {:6.3f} ms",
			idx,
			t.count(),
			costMS);

		std::this_thread::sleep_for(next);
	}

private:
	duration<double> ft; // 每帧目标时长 (秒)
	steady_clock::time_point start;
};
