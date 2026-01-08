#pragma once

#include "../util/pose.hpp"
#include "../util/sprite.hpp"
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

	SpriteBox() {};

	bool import(const pb::SpriteBox &sc,
		SDL_Renderer *r,
		const std::filesystem::path &dir) {

		for (const auto &row : sc.sprite()) {

			auto sp = std::make_shared<Sprite>();
			sp->pose = row.pose();

			auto file = dir / row.path();
			sp->list = util::loadSpriteFrames(r, file);
			if (sp->list.empty()) {
				spdlog::warn(
					"sprite character {} is empty: {}", name, file.string());
				return false;
			}

			for (int s : row.step()) {
				sp->duration.push_back(s);
			}

			auto size = sp->list.size();
			if (size == 0 || size != sp->duration.size()) {
				spdlog::error(
					"sprite character {}.{} has invalid step size: {} vs {}",
					name,
					util::poseName(row.pose()),
					size,
					sp->duration.size());
				return false;
			}

			sprite.insert({sp->pose, sp});
		}
		name = sc.name();
		return true;
	};
	~SpriteBox() {
		sprite.clear();
	};
};

}; // namespace asset
