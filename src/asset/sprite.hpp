#pragma once

#include "../name/sprite.hpp"
#include "common.hpp"
#include "pb/pose.pb.h"
#include <SDL3_image/SDL_image.h>
#include <unordered_map>

namespace asset {

struct Sprite {
	pb::Pose::Type pose;
	std::vector<SDL_Texture *> list = {};
	std::vector<int> duration = {};

	Sprite(const pb::Pose::Type &pose_) : pose(pose_) {};

	SDL_Texture *texture(int step) const {
		return list[step];
	};

	~Sprite() {
		for (auto texture : list) {
			SDL_DestroyTexture(texture);
		}
		list.clear();
		duration.clear();
	};
};

struct [[nodiscard]] SpriteBox {
	const name::Sprite name;
	std::unordered_map<pb::Pose::Type, Sprite> sprite = {};
	Size physics = {};
	VisualSize visual = {};

	SpriteBox(const name::Sprite &name_) : name(name_) {};

	const Sprite &get(const pb::Pose::Type &t) const {
		return sprite.at(t);
	};

	~SpriteBox() {
		sprite.clear();
	};
};

struct SpriteRef {
	const asset::SpriteBox &ptr;
};

}; // namespace asset
