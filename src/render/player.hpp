#pragma once

#include "../common/pose.hpp"
#include "../util/sprite.hpp"
#include "base.hpp"
#include <SDL3_image/SDL_image.h>
#include <spdlog/spdlog.h>

namespace render {

struct Player : base {

	using base::base;

	void init() override {

		const std::unordered_map<Pose::Type, std::string> li = {
			{Pose::Type::Walk, "walk"},
			{Pose::Type::Run, "run"},
			{Pose::Type::Attack1, "attack_1"},
			{Pose::Type::Attack2, "attack_2"},
			{Pose::Type::Attack3, "attack_3"},
			{Pose::Type::Jump, "jump"},
			{Pose::Type::Idle, "idle"},
		};

		for (const auto &[type, name] : li) {
			const std::filesystem::path file =
				"asset/character/samurai/" + name + ".png";
			auto frames = util::loadSpriteFrames(r, file);
			if (frames.empty()) {
				spdlog::error("Failed to load player sprites file: {}", name);
				return;
			}
			texMap.insert({type, frames});
			spdlog::info("Loaded player {} sprites: {}", name, frames.size());
		}
	};
	void render() override {

		if (!d.scene) {
			spdlog::error("d.scene is nullptr!");
			return;
		}

		float size = 500.0f;

		const auto &pose = d.scene->player.pose;
		const auto &tex = texMap.at(pose.type);

		int step = pose.step % tex.size();

		SDL_FRect dst = {200.0f, 200.0f, size, size};

		if (d.scene->player.pose.facing == Pose::Facing::Left) {
			SDL_FlipMode flip = SDL_FLIP_HORIZONTAL;
			SDL_RenderTextureRotated(
				r, tex[step], nullptr, &dst, 0.0, nullptr, flip);
		} else {
			SDL_RenderTexture(r, tex[step], nullptr, &dst);
		}
	};
};
}; // namespace render
