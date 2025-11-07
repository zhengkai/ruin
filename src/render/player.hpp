#pragma once

#include "../common/pose.hpp"
#include "../util/sprite.hpp"
#include "base.hpp"
#include <SDL3_image/SDL_image.h>
#include <spdlog/spdlog.h>

namespace render {

struct Player : base {

	using base::base;

	std::unordered_map<Pose::Type, std::vector<SDL_Texture *>> texMap = {};

	void init() override {

		const std::unordered_map<Pose::Type, std::string> li = {
			{Pose::Type::Walk, "Walk"},
			{Pose::Type::Run, "Run"},
			{Pose::Type::Attack, "Attack_1"},
			{Pose::Type::Jump, "Jump"},
			{Pose::Type::Idle, "Idle"},
		};

		for (const auto &[type, name] : li) {
			const std::filesystem::path file =
				"assets/Samurai/" + name + ".png";
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
		spdlog::info("start render a");

		spdlog::info("pose.type = {}", static_cast<int>(pose.type));
		spdlog::info("start render b");

		const auto &tex = texMap.at(pose.type);
		spdlog::info("start render tex");

		int step = pose.step % tex.size();

		SDL_FRect dst = {200.0f, 200.0f, size, size};
		SDL_RenderTexture(r, tex[step], nullptr, &dst);
	};
};
}; // namespace render
