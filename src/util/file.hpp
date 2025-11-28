#pragma once

#include <fstream>
#include <google/protobuf/util/json_util.h>
#include <spdlog/spdlog.h>
#include <string>

namespace util {

inline std::string loadJSONFile(std::string path) {
	std::ifstream ifs(path);
	if (!ifs.is_open()) {
		return "";
	}
	std::string j((std::istreambuf_iterator<char>(ifs)),
		std::istreambuf_iterator<char>());
	if (ifs.fail() && !ifs.eof()) {
		return "";
	}
	return j;
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
