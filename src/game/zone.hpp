#pragma once

#include "../asset/asset.hpp"
#include "../asset/sprite.hpp"
#include "../common/pose.hpp"
#include "../context/window.hpp"
#include "../physics/body.hpp"
#include "../physics/physics.hpp"
#include "../physics/rect.hpp"
#include "../tag.hpp"
#include "../util/animation.hpp"
#include "player.hpp"
#include "reg.hpp"

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
			reg.emplace<tag::AssetMapCell>(e, t);
		}

		for (auto &m : m->monster) {
			auto e = reg.create();
			reg.emplace<tag::Monster>(e);
			reg.emplace<physics::Rect>(e, m);
			reg.emplace<physics::Body>(e);
			reg.emplace<Pose>(e);
			reg.emplace<tag::PrevPos>(e, m.x, m.y);
			reg.emplace<std::shared_ptr<asset::SpriteBox>>(e, m.def.sprite);
		}
	};

	void enter(physics::Pos pos) {
		player = reg.create();
		reg.emplace<tag::Player>(player);
		reg.emplace<physics::Rect>(
			player, pos, config::playerW, config::playerH);
		reg.emplace<physics::Body>(player);
		reg.emplace<Pose>(player);
		reg.emplace<tag::PrevPos>(player, pos.x, pos.y);
		reg.emplace<std::shared_ptr<asset::SpriteBox>>(
			player, asset.sprite.at("samurai"));
	}

	void leave() {
		reg.destroy(player);
	};

	void step(const context::Control &control) {

		auto v3 = reg.view<physics::Body, tag::Player>();
		for (auto [e, body] : v3.each()) {
			stepPlayer(e, control, body);
		}

		physics.step();

		auto v1 = reg.view<physics::Rect, tag::PrevPos>();
		for (auto [_, rect, pos] : v1.each()) {
			pos.x = rect.x;
			pos.y = rect.y;
		}

		auto v2 = reg.view<Pose, std::shared_ptr<asset::SpriteBox>>();
		for (auto [_, pose, box] : v2.each()) {
			util::animation(pose, box);
		}
	};

	void stepPlayer(
		entt::entity &e, const context::Control &control, physics::Body &body) {

		playerJump(reg, e, control, body);
	};

	const Reg &getReg() const {
		return reg;
	};

	const entt::entity &getPlayer() const {
		return player;
	};
};

}; // namespace game
