#pragma once

#include "Gui.h"

namespace Salad {

	class GuiManager {

	public:
		GuiManager();

		int addWindow();
		void removeWindow(int windowHandle);

	private:
		std::vector<GuiWindow> m_Windows;

		int m_Focus;

	};

}
