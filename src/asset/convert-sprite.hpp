#pragma once

#include "../util/pose.hpp"
#include "../util/sprite.hpp"
#include "common.hpp"
#include "pb/asset.pb.h"
#include "sprite.hpp"
#include <SDL3_image/SDL_image.h>
#include <unordered_map>

namespace asset {

bool importOneSprite(const pb::Sprite &src,
	Sprite &dst,
	SDL_Renderer *r,
	const std::filesystem::path &file,
	const std::string &name) {

	dst.list = util::loadSpriteFrames(r, file);
	if (dst.list.empty()) {
		spdlog::warn("sprite character {} is empty: {}", name, file.string());
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

bool convertSpriteBox(const pb::SpriteBox &src,
	SpriteBox &dst,
	SDL_Renderer *r,
	const std::filesystem::path &dir) {

	dst.name = src.name();

	for (const auto &row : src.sprite()) {

		auto pose = row.pose();

		dst.sprite.emplace(pose, pose);
		auto &sp = dst.sprite.at(pose);

		auto file = dir / row.path();
		if (!importOneSprite(row, sp, r, file, dst.name)) {
			return false;
		}
	}

	if (!importSpriteMeta(src, dst)) {
		return false;
	}

	return true;
};

}; // namespace asset
