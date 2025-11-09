#pragma once

#include "../config.hpp"
#include "../util/file.hpp"
#include "../util/path.hpp"
#include "../util/sprite.hpp"
#include "asset.hpp"
#include "pb/asset.pb.h"
#include <google/protobuf/util/json_util.h>

namespace asset {

inline void mergeCharacter(
	SDL_Renderer *r, pb::AssetManifest &src, Asset &dst) {

	for (const auto &sc : src.character()) {

		auto dc = std::make_shared<Character>();
		dc->name = sc.name();
		dst.character[dc->name] = dc;

		std::cout << "Character name: " << sc.name() << std::endl;
		for (const auto &row : sc.sprite()) {

			auto sp = std::make_shared<Sprite>();
			sp->pose = row.pose();

			auto file = util::file(row.path());
			sp->list = util::loadSpriteFrames(r, file);

			for (int s : row.step()) {
				sp->duration.push_back(s);
			}
			dc->sprite.insert({sp->pose, sp});

			auto pose = pb::Pose::Type_descriptor()
							->FindValueByNumber(row.pose())
							->name();

			std::cout << "  Sprite type: " << row.pose() << pose
					  << ", path: " << row.path() << ", steps: ";
			for (auto s : row.step())
				std::cout << s << " ";
			std::cout << std::endl;
		}
	}
}

inline void Load(SDL_Renderer *r, std::filesystem::path dir, Asset &asset) {

	auto f = util::file(dir / "manifest.json");
	auto s = util::loadJSONFile(f);

	pb::AssetManifest manifest;
	google::protobuf::util::JsonParseOptions options;
	options.ignore_unknown_fields = true;

	auto status =
		google::protobuf::util::JsonStringToMessage(s, &manifest, options);
	if (!status.ok()) {
		spdlog::error("Failed to JSON {}, error {}", f, status.message());
		return;
	}

	mergeCharacter(r, manifest, asset);

	spdlog::info("Loading asset from {}", f);
};
}; // namespace asset
