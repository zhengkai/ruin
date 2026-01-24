#pragma once

#include "../config.hpp"
#include <algorithm>
#include <cstddef>
#include <numeric>
#include <vector>

namespace context {

struct FrameStats {
	std::size_t min;
	std::size_t max;
	std::size_t avg;
	std::size_t miss;
};

struct FrameMonitor {
	std::vector<std::size_t> timeUsBuffer; // 每帧微秒时间，环形
	std::size_t idx = 0;
	std::size_t missCnt = 0;
	bool isInit = false;

	FrameMonitor() : timeUsBuffer(config::frameBufferSize, 0) {};

	void pushTimeUs(std::size_t us) {

		idx++;
		if (idx >= config::frameBufferSize) {
			idx = 0;
		}
		if (us > config::frameTimeUs) {
			missCnt++;
		}
		timeUsBuffer[idx] = us;
	};

	FrameStats getStats() const {
		auto [min, max] =
			std::minmax_element(timeUsBuffer.begin(), timeUsBuffer.end());

		return {
			.min = *min,
			.max = *max,
			.avg =
				std::accumulate(
					timeUsBuffer.begin(), timeUsBuffer.end(), std::size_t{0}) /
				config::frameBufferSize,
			.miss = missCnt,
		};
	};
};
}; // namespace context
