#pragma once

#include "Salad/Core/Core.h"
#include "Salad/Renderer/Texture.h"

#include <string>
#include <vector>
#include <stack>

namespace Salad {

	enum class FileExplorerItemType {
		Unknown = 255,
		Folder = 0,
		Model = 1,
		Texture = 2,
		GLSL = 3,
		TrueTypeFont = 4,
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

		void onItemDoubleClicked(FileExplorerItem* item);
		//void onItemRightClicked(FileExplorerItem* item);

		void refresh();

	private:
		std::string m_CurrentDirectory;
		std::vector<FileExplorerItem*> m_Items;
		
		Ref<Texture2D> m_FileIconTextureMap;
		int m_Selected = -1;
		
		std::stack<std::string> m_PreviousDirectories;
	};



}