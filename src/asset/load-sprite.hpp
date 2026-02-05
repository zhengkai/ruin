#pragma once

#include "../name/sprite.hpp"
#include "../util/pose.hpp"
#include "asset.hpp"
#include "common.hpp"
#include "file-loader.hpp"
#include "sprite.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <unordered_map>

namespace asset {

inline std::vector<SDL_Texture *> loadSpriteFrames(
	SDL_Renderer *r, Texture &t) {

	std::vector<SDL_Texture *> fl;
	SDL_FRect dst{0.0f, 0.0f, t.h, t.h};

	int h = static_cast<int>(t.h);

	for (float x = 0.0f; x < t.w; x += t.h) {
		SDL_FRect src{x, 0, t.h, t.h};
		SDL_Texture *f = SDL_CreateTexture(
			r, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, h, h);

		SDL_SetRenderTarget(r, f);
		SDL_RenderTexture(r, t.texture, &src, &dst);
		SDL_SetRenderTarget(r, nullptr);
		fl.push_back(f);
	}

	return fl;
};

bool importOneSprite(const pb::Sprite &src,
	Sprite &dst,
	Texture &t,
	SDL_Renderer *r,
	const name::Sprite &name) {

	dst.list = loadSpriteFrames(r, t);
	if (dst.list.empty()) {
		spdlog::warn("sprite character {} is empty", name);
		return false;
	}

	for (int s : src.step()) {
		dst.duration.push_back(s);
	}

	auto size = dst.list.size();
	if (size == 0 || size != dst.duration.size()) {
		spdlog::error("sprite character {}.{} has invalid step size: {} vs {}",
			name,
			util::poseName(src.pose()),
			size,
			dst.duration.size());
		return false;
	}

	return true;
};

bool importSpriteMeta(const pb::SpriteBox &src, SpriteBox &dst) {

	dst.physics.w = src.physics().w() * 0.5f;
	dst.physics.h = src.physics().h() * 0.5f;
	if (!dst.physics.w || !dst.physics.h) {
		spdlog::warn("sprite {} has invalid physics size: {}x{}",
			dst.name,
			dst.visual.w,
			dst.visual.h);
		return false;
	}

	dst.visual.w = src.visual().w();
	dst.visual.h = src.visual().w();
	if (!dst.visual.w || !dst.visual.h) {
		spdlog::warn("sprite {} has invalid visual size: {}x{}",
			dst.name,
			dst.visual.w,
			dst.visual.h);
		return false;
	}
	dst.visual.offsetY = src.visual().offsety();
	return true;
};

bool convertSpriteBox(
	const pb::SpriteBox &src, SpriteBox &dst, FileLoader &loader) {

	for (const auto &row : src.sprite()) {

		auto pose = row.pose();

		dst.sprite.emplace(pose, pose);
		auto &sp = dst.sprite.at(pose);

		auto t = Texture{};
		if (!loader.load(row.path(), t)) {
			return false;
		}

		if (!importOneSprite(row, sp, t, loader.r, dst.name)) {
			spdlog::warn("sprite box {} load fail: {}", dst.name, row.path());
			return false;
		}
	}

	if (!importSpriteMeta(src, dst)) {
		return false;
	}

	return true;
};

void convertSprite(const google::protobuf::RepeatedPtrField<pb::SpriteBox> list,
	Asset &dst,
	FileLoader &loader) {
	for (const auto &sc : list) {

		auto name = name::Sprite{sc.name()};
		if (dst.sprite.contains(name)) {
			spdlog::warn("duplicate sprite name: {}", name);
			return;
		}
		dst.sprite.emplace(name, name);
		auto &b = dst.sprite.at(name);
		convertSpriteBox(sc, b, loader);
	}
};

}; // namespace asset
