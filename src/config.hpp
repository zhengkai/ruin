#pragma once

#include <SDL3/SDL_pixels.h>
#include <filesystem>
#include <spdlog/spdlog.h>

namespace config {

const float halfFloat = 0.49609375; // 127/256
const float halfFloatNegative = 0.0f - halfFloat;

inline bool fullscreen = false;
inline bool versionOnly = false;
inline bool helpOnly = false;
inline std::string verbose = "info";

inline std::vector<std::filesystem::path> dirList;

inline const std::filesystem::path assetDir = "asset";
inline const std::filesystem::path assetManifest = "manifest.json";

const float speed = 30.0f;
const float speedMax = speed * 1.2f;
const float speedMin = speed / 1.2f;

const int fps = 120;
const float deltaTime = 1.0f / static_cast<float>(fps);

const float gravity = 0.4f * deltaTime;
const float gravityUp = 0.3f * deltaTime;
const float jumpForce = 16.0f * deltaTime;
const float jumpShortForce =
	jumpForce * 0.5f; // 小跳，跳跃键松开时向上力不得超过此数
const float maxJumpCount = 2;
const float downSpeedMax = -1.25f;

const float posResetX = 13.0f;
const float posResetY = 11.0f;

const float physicsScanRange =
	5.0f; // 坐标差别超过这个的物体（无论大小）直接不检测碰撞

inline const char *winTitle = "Ruin Test";

const SDL_Color colorBg = {16, 64, 128, 255};

const float brickBorder = 1.0f;

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

enum class ColorTheme {
	HCT,
	Map,
	Rand,
};
inline ColorTheme colorTheme = ColorTheme::HCT;

const int cdJump = fps / 4;
const int cdZoom = 5;
const int cdFullscreen = fps / 5;

const float focusSpeed = 0.05f;
const float focusRange = 3.0f;

}; // namespace config

#ifdef __EMSCRIPTEN__
constexpr bool cfgWASM = true;
#else
constexpr bool cfgWASM = false;
#endif

constexpr int cfgPhyLoop = 1;
