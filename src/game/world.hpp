#pragma once

#include "../asset/asset.hpp"
#include "../physics/rect.hpp"
#include "tag.hpp"
#include <entt/entt.hpp>

namespace game {

class World {

private:
	entt::registry reg;
	entt::entity player;

public:
	const asset::Asset &asset;
	const std::string name;

public:
	World(const std::string &name_, const asset::Asset &asset_)
		: asset(asset_), name(name_) {

		player = reg.create();
		reg.emplace<physics::Rect>(player, 10.0f, 10.0f, 0.5f);

		auto m = asset.map.at(name);
		for (auto &t : m->terrain) {
			auto e = reg.create();
			reg.emplace<physics::Rect>(e, t.pos, 0.5f);
			reg.emplace<AssetMapCell>(e, t);
		}
	};

	void enter(physics::Pos pos) {
		spdlog::info("enter map {}", name);
		auto &rect = reg.get<physics::Rect>(player);
		spdlog::info("enter map done {}", name);
		rect.updatePos(pos);
	}
};

}; // namespace game
