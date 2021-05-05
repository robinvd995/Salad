#include "sldpch.h"
#include "Salad/Util/PlatformUtils.h"

#include "Salad/Core/Application.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>


namespace Salad {

	std::string FileDialog::openFileDialog(const char* filter) {
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::get().getWindow().getNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if(GetOpenFileNameA(&ofn) == TRUE) {
			return ofn.lpstrFile;
		}
		else{
			return std::string();
		}
	}

	std::string FileDialog::saveFileDialog(const char* filter) {
		return "";
	}

}