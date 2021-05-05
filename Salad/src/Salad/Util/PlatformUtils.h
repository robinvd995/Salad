#pragma once

#include <string>

namespace Salad {

	class FileDialog {
	
	public:
		static std::string openFileDialog(const char* filter);
		static std::string saveFileDialog(const char* filter);
	
	};

}