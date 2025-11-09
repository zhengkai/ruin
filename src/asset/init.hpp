#pragma once

#include "../util/file.hpp"
#include "../util/path.hpp"
#include "../util/sprite.hpp"
#include "asset.hpp"
#include "pb/asset.pb.h"
#include "util.hpp"
#include <google/protobuf/util/json_util.h>

namespace asset {

inline bool mergeCharacter(SDL_Renderer *r,
	pb::AssetManifest &src,
	std::filesystem::path dir,
	Asset &dst) {

	for (const auto &sc : src.character()) {

		auto dc = std::make_shared<Character>();
		dc->name = sc.name();
		dst.character[dc->name] = dc;

		for (const auto &row : sc.sprite()) {

			auto sp = std::make_shared<Sprite>();
			sp->pose = row.pose();

			auto file = dir / row.path();
			sp->list = util::loadSpriteFrames(r, file);
			if (sp->list.empty()) {
				spdlog::warn("sprite character {} is empty: {}",
					dc->name,
					file.string());
				return false;
			}

			for (int s : row.step()) {
				sp->duration.push_back(s);
			}

			auto size = sp->list.size();
			if (size == 0 || size != sp->duration.size()) {
				spdlog::error(
					"sprite character {}.{} has invalid step size: {} vs {}",
					dc->name,
					poseName(row.pose()),
					size,
					sp->duration.size());
				return false;
			}

			dc->sprite.insert({sp->pose, sp});
		}
	}
	return true;
}

inline bool loadJSON(std::string file, pb::AssetManifest &manifest) {

	auto s = util::loadJSONFile(file);
	if (s.empty()) {
		spdlog::error("Failed to load asset manifest: {}", file);
		return false;
	}

	google::protobuf::util::JsonParseOptions options;
	options.ignore_unknown_fields = true;

	auto status =
		google::protobuf::util::JsonStringToMessage(s, &manifest, options);
	if (!status.ok()) {
		spdlog::error("Failed to JSON {}, error: {}", file, status.message());
		return false;
	}
	return true;
}

inline bool Load(SDL_Renderer *r, std::filesystem::path dir, Asset &asset) {

	auto f = util::file(dir / "manifest.json");
	pb::AssetManifest manifest;
	if (!loadJSON(f, manifest)) {
		return false;
	}

	if (!mergeCharacter(r, manifest, dir, asset)) {
		return false;
	}

	spdlog::info("Loading asset from {}", f);
	return true;
};
}; // namespace asset
