#pragma once

#include <string>
#include <algorithm>
#include <fstream>

#define SLD_FILE_PATH_SEPERATOR '/'

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

	inline std::string fileNameFromPath(const std::string& path) {
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

	inline bool fileExtensionFromName(const std::string& filename, std::string* extension) {
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

	inline std::string subPath(const std::string& path, uint32_t off, uint32_t size = 0) {

		int count = 0;
		int start = -1;
		uint32_t end = UINT_MAX;;

		if (off == 0) start = 0;

		for(int i = 0; i < path.size(); i++) {
			char c = path[i];
			if(c == SLD_FILE_PATH_SEPERATOR) {
				count++;
				if(count >= off && start == -1) {
					start = i + 1;
				}

				if(count == size) {
					end = start - i;
					break;
				}
			}
		}

		return path.substr(start, end);
	}

	inline void replaceFileSeperators(std::string& path, char replacement) {
		std::replace(path.begin(), path.end(), SLD_FILE_PATH_SEPERATOR, replacement);
	}

	inline std::string popExtension(std::string& path) {
		size_t index = path.find_last_of('.');
		if (index == std::string::npos) return path;
		return path.substr(0, index);
	}

	inline std::string popExtension(const std::string& path) {
		size_t index = path.find_last_of('.');
		if (index == std::string::npos) return path;
		return path.substr(0, index);
	}

	inline std::string popAllExtensions(std::string& path) {
		size_t index = path.find_first_of('.');
		if (index == std::string::npos) return path;
		return path.substr(0, index);
	}
}
