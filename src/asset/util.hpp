#pragma once

#include "../util/pos.hpp"
#include "asset.hpp"
#include "map.hpp"
#include "pb/map.pb.h"
#include "texture.hpp"
#include <SDL3_image/SDL_image.h>

namespace asset {

struct FileLoader {
	SDL_Renderer *r;
	std::filesystem::path dir;

	FileLoader(SDL_Renderer *r_, std::filesystem::path dir_)
		: r{r_}, dir{dir_} {};

	bool load(const std::string &path, Texture &t) {
		auto file = dir / path;

#ifdef _MSC_VER
	auto u8 = file.generic_u8string(); // std::u8string
	std::string p(u8.begin(), u8.end()); // char8_t -> char (逐字节拷贝)
	SDL_Surface *s = IMG_Load(p.c_str());
#else
	SDL_Surface *s = IMG_Load(file.c_str());
#endif

		if (!s) {
			spdlog::error("load image fail: {}", path);
			return false;
		}

		t.w = static_cast<float>(s->w);
		t.h = static_cast<float>(s->h);
		t.texture = SDL_CreateTextureFromSurface(r, s);
		SDL_DestroySurface(s);

		return true;
	}
};

void convertPBTriggerGate(
	const uint32_t tid, const pb::MapTriggerGate &g, Zone &z, MapGate &mg) {

	int id = static_cast<int>(tid);
	mg.id = id;
	mg.rect = util::convertIDToRect(id, z.map);
	mg.target = {
		{g.x(), g.y()},
		g.zone(),
	};
};

void convertMapStaticTerrain(
	Map &m, const google::protobuf::RepeatedPtrField<pb::MapCell> &li) {

	std::unordered_map<std::size_t, const pb::MapCell &> tile = {};
	tile.reserve(li.size());
	for (const auto &s : li) {
		tile.emplace(static_cast<std::size_t>(s.id()), s);
	}

	for (std::size_t y = 0; y < m.h; y++) {
		for (std::size_t x = 0; x < m.w; x++) {
			auto id = y * m.w + x;
			auto &c = m.terrain.emplace_back();
			if (tile.contains(id)) {
				auto &t = tile.at(id);
				auto i = static_cast<int>(id);
				c.tileName = t.tile().name();
				c.tileID = static_cast<int>(t.tile().id());
				c.pos = util::convertIDToPos(i, m);
			}
		}
	}
};

bool convertZoneBackground(Zone &z,
	FileLoader &loader,
	const google::protobuf::RepeatedPtrField<pb::ZoneBackground> &li) {

	z.background.reserve(li.size());

	for (int i = li.size() - 1; i >= 0; --i) {
		const auto &b = li.Get(i);
		auto &bg = z.background.emplace_back();

		switch (b.type_case()) {
		case pb::ZoneBackground::kTile:
			bg.loopCnt = b.tile().loopcnt();
			if (bg.loopCnt <= 0.0f) {
				bg.loopCnt = 1.0f;
			}
			break;
		default:
			break;
		}

		if (!loader.load(b.path(), bg)) {
			return false;
		}
	}

	return true;
};

bool convertMapTrigger(
	Zone &z, const google::protobuf::RepeatedPtrField<pb::MapTrigger> &li) {
	for (const auto &t : li) {
		switch (t.trigger_case()) {
		case pb::MapTrigger::kGate: {
			auto &mg = z.gate.emplace_back();
			convertPBTriggerGate(t.id(), t.gate(), z, mg);
			break;
		}
		case pb::MapTrigger::kExit: {
			auto &mg = z.exit.emplace_back();
			convertPBTriggerGate(t.id(), t.gate(), z, mg);
			break;
		}
		case pb::MapTrigger::TRIGGER_NOT_SET:
		default:
			spdlog::info("unknown trigger");
			return false;
			break;
		}
	}
	return true;
};

bool convertMapMob(Zone &z,
	Asset &dst,
	const google::protobuf::RepeatedPtrField<pb::MapMob> &li) {

	int index = -1;
	for (const auto &c : li) {
		index++;

		auto type = name::MobType{c.type()};
		if (!dst.mob.contains(type)) {
			spdlog::error(
				"mob type not found at zone {}({}.{}), index {}: {} {}",
				z.name,
				c.x(),
				c.y(),
				index,
				type,
				dst.mob.size());
			return false;
		}

		auto spriteName = name::Sprite{c.sprite()};

		auto pos = physics::Pos{
			.x = c.x(),
			.y = c.y(),
		};

		z.mob.emplace_back(type,
			spriteName ? dst.sprite.at(spriteName) : dst.mob.at(type).sprite,
			pos);
	}
	return true;
};

void convertMap(const pb::Map &pm, Map &m) {

	m.w = static_cast<std::size_t>(pm.w());
	m.h = static_cast<std::size_t>(pm.h());

	convertMapStaticTerrain(m, pm.terrain());
};

bool convertZone(const pb::Zone &pm, FileLoader &fl, Asset &dst, Zone &z) {

	if (!convertZoneBackground(z, fl, pm.background())) {
		return false;
	}
	if (!convertMapTrigger(z, pm.trigger())) {
		return false;
	}
	if (!convertMapMob(z, dst, pm.mob())) {
		return false;
	}
	return true;
};
}; // namespace asset
