#pragma once

#include <fstream>
#include <iostream>
#include <string>

namespace util {

inline std::string loadJSONFile(std::string path) {
	std::ifstream ifs(path);
	if (!ifs.is_open()) {
		std::cerr << "Failed to open /tmp/foo.json" << std::endl;
		return "";
	}
	std::string json_str((std::istreambuf_iterator<char>(ifs)),
		std::istreambuf_iterator<char>());
	return json_str;
};
}; // namespace util
