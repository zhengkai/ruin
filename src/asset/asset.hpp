#pragma once

#include "../config.hpp"
#include "../util/file.hpp"
#include "../util/path.hpp"
#include "pb/asset.pb.h"
#include <google/protobuf/util/json_util.h>

inline void LoadAsset() {

	auto f = util::file(config::assetDir / "manifest.json");

	auto s = util::loadJSONFile(f);

	spdlog::info("json: {}", s);

	pb::AssetManifest manifest;
	google::protobuf::util::JsonParseOptions options;
	options.ignore_unknown_fields = true;

	auto status =
		google::protobuf::util::JsonStringToMessage(s, &manifest, options);
	if (!status.ok()) {
		spdlog::error("Failed to JSON {}, error {}", f, status.message());
		return;
	}

	for (const auto &character : manifest.character()) {
		std::cout << "Character name: " << character.name() << std::endl;
		for (const auto &sprite : character.sprite()) {
			std::cout << "  Sprite type: " << sprite.type()
					  << ", path: " << sprite.path() << ", steps: ";
			for (auto s : sprite.step())
				std::cout << s << " ";
			std::cout << std::endl;
		}
	}

	spdlog::info("Loading asset from {}", f);
}
