#pragma once

#include <google/protobuf/util/json_util.h>
#include <spdlog/spdlog.h>
#include <string>

namespace util {

inline std::string loadJSONFile(const std::string &path) {

	SDL_IOStream *io = SDL_IOFromFile(path.c_str(), "rb");
	if (!io) {
		return {};
	}

	Sint64 size = SDL_GetIOSize(io);
	std::string buf(size, '\0');
	SDL_ReadIO(io, buf.data(), size);
	SDL_CloseIO(io);

	return buf;
};

inline bool loadJSON(std::string file, google::protobuf::Message &m) {

	auto s = util::loadJSONFile(file);
	if (s.empty()) {
		spdlog::error("Failed to load json: {}", file);
		return false;
	}

	google::protobuf::util::JsonParseOptions options;
	options.ignore_unknown_fields = true;

	auto status = google::protobuf::util::JsonStringToMessage(s, &m, options);
	if (!status.ok()) {
		spdlog::error("Failed to JSON {}, error: {}", file, status.message());
		return false;
	}
	return true;
}
}; // namespace util
