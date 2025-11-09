#pragma once

#include <fstream>
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
}; // namespace util
