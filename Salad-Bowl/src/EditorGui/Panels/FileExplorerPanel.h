#pragma once

#include "Salad/Core/Core.h"
#include "Salad/Renderer/Texture.h"

#include <string>
#include <vector>
#include <stack>

namespace Salad::EditorGui {

	enum class FileExplorerItemType {
		Unknown = 255,
		Folder = 0,
		Model = 1,
		Texture = 2,
		GLSL = 3,
		TrueTypeFont = 4,
		SharpScript = 5,
		LuaScript = 6
	};

	struct FileExplorerItem {
		std::string filename;
		std::string path;
		FileExplorerItemType type = FileExplorerItemType::Unknown;
		uint32_t fileExplorerViewId;

		FileExplorerItem() = delete;
		FileExplorerItem(uint32_t viewid) : fileExplorerViewId(viewid) {}

		friend class FileExplorerPanel;
	};

	class FileExplorerPanel {
	
	public:

		FileExplorerPanel() {}
		virtual ~FileExplorerPanel() {}

		void init();
		void onImGuiRender();

	private:

		void scopeToFolder(std::string& path, bool pushPrev = true);
		void scopeToPrevious();
		void scopeToParent();

		void onItemDoubleClicked(FileExplorerItem* item);
		//void onItemRightClicked(FileExplorerItem* item);

		void refresh();

		void updateIgnoredExtensions();

	private:
		std::string m_CurrentDirectory;
		std::vector<FileExplorerItem*> m_Items;
		int m_Selected = -1;
		
		std::stack<std::string> m_PreviousDirectories;

		std::vector<std::string> m_ExtensionIgnoreList;

		std::string m_IgnoredExtensions = "asset";
		std::string m_PreviousIgnoredExtensions;

		bool m_IgnoreUnknownFiles = false;
		bool m_AssetStateIconShouldDraw[4]{};
		char m_StringBuffer[256]{};
		std::string m_Filter;
	};

}
