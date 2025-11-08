#pragma once

#include "../config.hpp"
#include "../util/file.hpp"
#include "../util/path.hpp"
#include "asset.hpp"
#include "pb/asset.pb.h"
#include <google/protobuf/util/json_util.h>

namespace asset {

inline void mergeCharacter(pb::AssetManifest &src, Asset &dst) {

	for (const auto &sc : src.character()) {

		auto dc = std::make_shared<Character>();
		dc->name = sc.name();

		std::cout << "Character name: " << sc.name() << std::endl;
		for (const auto &sprite : sc.sprite()) {

			auto pose = pb::Pose::Type_descriptor()
							->FindValueByNumber(sprite.pose())
							->name();

			std::cout << "  Sprite type: " << sprite.pose() << pose
					  << ", path: " << sprite.path() << ", steps: ";
			for (auto s : sprite.step())
				std::cout << s << " ";
			std::cout << std::endl;
		}
	}
}

inline void Load(std::filesystem::path dir, Asset &asset) {

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

	mergeCharacter(manifest, asset);

	spdlog::info("Loading asset from {}", f);
};

inline void Init() {
	Load(config::assetDir, asset);
};
}; // namespace asset
