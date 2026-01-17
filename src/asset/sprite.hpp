#pragma once

#include "../util/pose.hpp"
#include "../util/sprite.hpp"
#include "common.hpp"
#include "pb/asset.pb.h"
#include "pb/pose.pb.h"
#include <SDL3_image/SDL_image.h>
#include <unordered_map>

namespace asset {

struct Sprite {
	pb::Pose::Type pose;
	std::vector<SDL_Texture *> list = {};
	std::vector<int> duration = {};

	~Sprite() {
		for (auto texture : list) {
			SDL_DestroyTexture(texture);
		}
		list.clear();
		duration.clear();
	}
};

struct SpriteBox {
	std::string name = "";
	std::unordered_map<pb::Pose::Type, std::shared_ptr<Sprite>> sprite = {};
	Size physics = {};
	VisualSize visual = {};

	SpriteBox() {};

	bool import(const pb::SpriteBox &sc,
		SDL_Renderer *r,
		const std::filesystem::path &dir) {

		name = sc.name();

		for (const auto &row : sc.sprite()) {

			auto sp = std::make_shared<Sprite>();
			sp->pose = row.pose();

			auto file = dir / row.path();
			if (!importOneSprite(row, sp, r, file)) {
				return false;
			}

			sprite.insert({sp->pose, sp});
		}

		if (!importSpriteMeta(sc)) {
			return false;
		}

		return true;
	};
	~SpriteBox() {
		sprite.clear();
	};

private:
	bool importSpriteMeta(const pb::SpriteBox &src) {

		physics.w = src.physics().w();
		physics.h = src.physics().h();
		if (!physics.w || !physics.h) {
			spdlog::warn("sprite {} has invalid physics size: {}x{}",
				name,
				visual.w,
				visual.h);
			return false;
		}

		visual.w = src.visual().w();
		visual.h = src.visual().w();
		if (!visual.w || !visual.h) {
			spdlog::warn("sprite {} has invalid visual size: {}x{}",
				name,
				visual.w,
				visual.h);
			return false;
		}
		visual.offsetY = src.visual().offsety();
		return true;
	};

	bool importOneSprite(const pb::Sprite &src,
		std::shared_ptr<Sprite> dst,
		SDL_Renderer *r,
		const std::filesystem::path &file) {

		dst->list = util::loadSpriteFrames(r, file);
		if (dst->list.empty()) {
			spdlog::warn(
				"sprite character {} is empty: {}", name, file.string());
			return false;
		}

		for (int s : src.step()) {
			dst->duration.push_back(s);
		}

		auto size = dst->list.size();
		if (size == 0 || size != dst->duration.size()) {
			spdlog::error(
				"sprite character {}.{} has invalid step size: {} vs {}",
				name,
				util::poseName(src.pose()),
				size,
				dst->duration.size());
			return false;
		}

		return true;
	};
};

}; // namespace asset
