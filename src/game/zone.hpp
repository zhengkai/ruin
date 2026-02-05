#pragma once

#include "../asset/asset.hpp"
#include "../asset/map.hpp"
#include "../asset/sprite.hpp"
#include "../asset/zone.hpp"
#include "../common/pose.hpp"
#include "../context/window.hpp"
#include "../decision/decision.hpp"
#include "../physics/body.hpp"
#include "../physics/physics.hpp"
#include "../physics/rect.hpp"
#include "../tag.hpp"
#include "../util/animation.hpp"
#include "../util/rand.hpp"
#include "player.hpp"
#include "reg.hpp"

namespace game {

class Zone {

private:
	Reg reg;
	entt::entity player = entt::null;
	physics::Physics physics;
	context::Window &window;

public:
	const asset::Zone &def;
	const asset::Asset &asset;

public:
	Zone(const name::Zone name_,
		const asset::Asset &asset_,
		context::Window &window_)
		: physics(asset_.zone.at(name_).map, reg), window(window_),
		  asset(asset_), def(asset_.zone.at(name_)) {

		for (auto &t : def.gate) {
			auto e = reg.create();
			reg.emplace<asset::MapGate>(e, t);
		}

		for (auto &m : def.mob) {
			auto e = reg.create();
			reg.emplace<tag::Mob>(e);
			reg.emplace<physics::Rect>(e, m.rect);
			reg.emplace<physics::Body>(e);
			reg.emplace<decision::Decision>(e, def.map);
			reg.emplace<Pose>(
				e, util::randBool() ? Pose::Facing::Left : Pose::Facing::Right);
			reg.emplace<tag::PrevPos>(e, m.rect.x, m.rect.y);
			reg.emplace<asset::SpriteRef>(e, asset::SpriteRef{m.sprite});
		}
	};

	void enter(physics::Pos pos) {

		spdlog::info("enter map {} {}.{}", def.name, pos.x, pos.y);

		if (reg.valid(player)) {
			spdlog::info("reset player pos");
			auto &rect = reg.get<physics::Rect>(player);
			rect.x = pos.x;
			rect.y = pos.y + rect.h - 0.5f; // TODO 怎么第一次图进不用修正
			reg.emplace<tag::PrevPos>(player, pos.x, pos.y);
			return;
		}

		window.camera.setMapSize(def.map);

		auto &sp = asset.sprite.at(asset.config.playerSprite);

		player = reg.create();
		reg.emplace<tag::Player>(player);

		reg.emplace<physics::Rect>(player, pos, sp.physics);

		reg.emplace<physics::Body>(player);
		reg.emplace<Pose>(player);
		reg.emplace<tag::PrevPos>(player, pos.x, pos.y);
		reg.emplace<asset::SpriteRef>(player, asset::SpriteRef{sp});
	}

	void leave() {
		reg.destroy(player);
	};

	void step(const context::Control &control) {

		auto v3 = reg.view<physics::Rect,
			tag::PrevPos,
			physics::Body,
			Pose,
			decision::Decision>();
		for (auto [e, rect, prevPos, body, pose, decision] : v3.each()) {
			stepMob(e, rect, prevPos, body, pose, decision);
		}

		auto v4 = reg.view<physics::Rect,
			tag::PrevPos,
			physics::Body,
			Pose,
			tag::Player>();
		for (auto [e, rect, prevPos, body, pose] : v4.each()) {
			stepPlayer(e, control, rect, prevPos, body, pose);
		}

		physics.step();

		auto v2 = reg.view<Pose, asset::SpriteRef>();
		for (auto [_, pose, box] : v2.each()) {
			util::animation(pose, box.ptr);
		}
	};

	void stepPlayer(entt::entity &e,
		const context::Control &control,
		const physics::Rect &rect,
		const tag::PrevPos &prevPos,
		physics::Body &body,
		Pose &pose) {

		if (control.btnU && playerEnterZone(reg, rect, window)) {
			return;
		};

		playerJump(reg, e, control, body);

		if (control.btnX && playerAttack(pose)) {
			// attack
		} else {
			playerPose(control, prevPos, rect, pose);
		}
	};

	void stepMob(entt::entity &e,
		const physics::Rect &rect,
		const tag::PrevPos &prevPos,
		physics::Body &body,
		Pose &pose,
		decision::Decision &decision) {

		decision.Do(rect, prevPos, body, pose);
	}

	const Reg &getReg() const {
		return reg;
	};

	const entt::entity &getPlayer() const {
		return player;
	};
};

}; // namespace game
