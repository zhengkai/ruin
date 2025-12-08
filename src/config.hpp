#pragma once

#include <SDL3/SDL_pixels.h>
#include <filesystem>
#include <numbers>
#include <spdlog/spdlog.h>

namespace config {

inline bool fullscreen = false;
inline bool versionOnly = false;
inline bool helpOnly = false;
inline std::string verbose = "info";

// classic: The ball only moves at 45-degree diagonal angles,
// similar to vnglst/ruin-wars
inline bool classic = false;

// 球半径小于 0.5 时，会出现长期垂直/水平方向运动，看起来不太自然
constexpr float ballRadius = 0.57f;

inline std::vector<std::filesystem::path> dirList;

inline const std::filesystem::path assetDir = "asset";
inline const std::filesystem::path assetManifest = "manifest.json";

const float speed = 30.0f;
const float speedMax = speed * 1.2f;
const float speedMin = speed / 1.2f;
const float speedClassic = 30.0f * std::numbers::sqrt2_v<float> / 2.0f;
const float gravity = speed * 10.0f;

const int fps = 60;
const float fpsDeltaTime = 1.0f / static_cast<float>(fps);

inline const char *winTitle = "Ruin Test";

const SDL_Color colorBg = {16, 64, 128, 255};

const float brickBorder = 1.0f;

inline std::vector<int> region;
inline int regionNum;
inline int ballNum;
const int regionNumDefault = 6;

const int gridWDefault = 60;
const int gridHDefault = 32;
inline int gridW;
inline int gridH;
inline float gridWF;
inline float gridHF;

inline int brickTotal;

inline float powerAvg;

const int speedLevelMax = 5; // 1/32 to 32x

inline void setGrid(int w, int h) {
	gridW = w;
	gridH = h;
	brickTotal = gridW * gridH;

	gridWF = static_cast<float>(gridW);
	gridHF = static_cast<float>(gridH);
}

inline void setRegion(std::vector<int> r) {
	region = r;
	regionNum = static_cast<int>(region.size());
	ballNum = 0;
	for (int i = 0; i < regionNum; i++) {
		ballNum += region[i];
	}
	powerAvg = static_cast<float>(brickTotal) / static_cast<float>(ballNum);
};

enum class ColorTheme {
	HCT,
	Map,
	Rand,
};
inline ColorTheme colorTheme = ColorTheme::HCT;

const int cdJump = 30;
const int cdZoom = 5;

const float focusSpeed = 0.05f;
const float focusRange = 3.0f;

}; // namespace config

#ifdef __EMSCRIPTEN__
constexpr bool cfgWASM = true;
#else
constexpr bool cfgWASM = false;
#endif

constexpr int cfgPhyLoop = 1;
