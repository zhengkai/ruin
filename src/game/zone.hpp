#pragma once

#include "../asset/asset.hpp"
#include "../asset/sprite.hpp"
#include "../common/pose.hpp"
#include "../physics/physics.hpp"
#include "../physics/rect.hpp"
#include "../util/animation.hpp"
#include "reg.hpp"
#include "tag.hpp"

namespace game {

class Zone {

private:
	Reg reg;
	entt::entity player;
	std::shared_ptr<asset::Map> m;
	physics::Physics physics;

public:
	const asset::Asset &asset;
	const std::string name;

public:
	Zone(const std::string &name_, const asset::Asset &asset_)
		: m(asset_.map.at(name_)), physics(m, reg), asset(asset_), name(name_) {

		for (auto &t : m->terrain) {
			auto e = reg.create();
			reg.emplace<physics::Rect>(e, t.pos, 0.5f);
			reg.emplace<AssetMapCell>(e, t);
		}

		for (auto &m : m->monster) {
			auto e = reg.create();
			reg.emplace<TagMonster>(e);
			reg.emplace<physics::Rect>(e, m);
			reg.emplace<physics::Body>(e);
			reg.emplace<Pose>(e);
			reg.emplace<std::shared_ptr<asset::SpriteBox>>(e, m.def.sprite);
		}
	};

	void enter(physics::Pos pos) {
		player = reg.create();
		reg.emplace<TagPlayer>(player);
		reg.emplace<physics::Rect>(player, pos, 0.5f);
		spdlog::info("insert reg body");
		reg.emplace<physics::Body>(player);
		reg.emplace<Pose>(player);
		reg.emplace<std::shared_ptr<asset::SpriteBox>>(
			player, asset.sprite.at("samurai"));
	}

	void leave() {
		reg.destroy(player);
	};

	void step() {
		auto view = reg.view<Pose, std::shared_ptr<asset::SpriteBox>>();
		for (auto [_, pose, box] : view.each()) {
			util::animation(pose, box);
		}
		physics.step();
	};

	const Reg &getReg() const {
		return reg;
	};

	const entt::entity &getPlayer() const {
		return player;
	};
};

}; // namespace game
