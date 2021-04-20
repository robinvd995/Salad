#pragma once

#include <string>
#include <fstream>

namespace Salad::FileUtil {

	inline std::string popDirectory(std::string& path) {
		size_t index = path.find_last_of("/", path.size() - 2);
		if (index == std::string::npos) return path;
		return path.substr(0, index);
	}

	inline std::string fileNameFromPath(std::string& path) {
		size_t index = path.find_last_of('/');
		if (index == std::string::npos) return path;
		return path.substr(index + 1);
	}

	inline bool fileExtensionFromName(std::string& filename, std::string* extension) {
		size_t index = filename.find_last_of('.');
		if (index == std::string::npos) return false;
		*extension = filename.substr(index + 1);
		return true;
	}

	inline bool fileExtensionFromPath(std::string& path, std::string* extension) {
		std::string filename = fileNameFromPath(path);
		return fileExtensionFromName(path, extension);
	}

	inline bool fileExists(std::string& path) {
		std::ifstream ifs(path);
		bool good = ifs.good();
		/*if(good) */ifs.close();
		return good;
	}

	inline bool fileExists(const std::string& path) {
		std::ifstream ifs(path);
		bool good = ifs.good();
		ifs.close();
		return good;
	}
}
