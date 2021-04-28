#include "FileExplorerPanel.h"

#include "imgui/imgui.h"

#include "Salad/Renderer/TextureManager.h"
#include "EditorSelectionContext.h"

#include <iostream>
#include <filesystem>
#include <map>
#include <algorithm>

#include "Util/FileUtils.hpp";

#include "EditorSettings.hpp"
#include "Assets/AssetSerializer.h"

namespace Salad {

	namespace Helper {

		// TODO: use the asset type here
		static const std::map<std::string, FileExplorerItemType> s_FileTypeMap{
			{"png", FileExplorerItemType::Texture},
			{"dae", FileExplorerItemType::Model},
			{"glsl", FileExplorerItemType::GLSL},
			{"ttf", FileExplorerItemType::TrueTypeFont},
			{"cs", FileExplorerItemType::SharpScript},
			{"lua", FileExplorerItemType::LuaScript}
		};

		static const std::map<FileExplorerItemType, std::pair<float, float>> s_FileTypeIconCoordinateMap{
			{ FileExplorerItemType::Unknown, { 0.5f, 0.0f } },
			{ FileExplorerItemType::Folder, { 0.0f, 0.0f } },
			{ FileExplorerItemType::Model, { 0.125f, 0.0f } },
			{ FileExplorerItemType::Texture, { 0.25f, 0.0f } },
			{ FileExplorerItemType::GLSL, { 0.375f, 0.0f } },
			{ FileExplorerItemType::TrueTypeFont, { 0.625f, 0.0f } },
			{ FileExplorerItemType::SharpScript, {0.75f, 0.0f} },
			{ FileExplorerItemType::LuaScript, {0.875f, 0.0f} }
		};

		FileExplorerItemType itemTypeFromExtension(std::string& extension) {
			auto it = s_FileTypeMap.find(extension);
			if (it != s_FileTypeMap.end()) return it->second;
			else return FileExplorerItemType::Unknown;
		}

		FileExplorerItemType itemTypeFromFilename(std::string& filename) {
			std::string extension;
			bool hasExtension = FileUtil::fileExtensionFromName(filename, &extension);
			if (hasExtension) {
				return itemTypeFromExtension(extension);
			}
			else {
				return FileExplorerItemType::Unknown;
			}
		}

		std::pair<float, float> textureCoordsForType(FileExplorerItemType type) {
			auto it = s_FileTypeIconCoordinateMap.find(type);
			if (it == s_FileTypeIconCoordinateMap.end()) return { 0.5f, 0.0f };
			return it->second;
		}

		bool shouldIgnoreExtension(std::vector<std::string>& ignoreList, std::string& extension) {
			for (std::string& ignore : ignoreList) {
				if (extension == ignore) return true;
			}
			return false;
		}

	}

	void FileExplorerPanel::init() {
		m_FileIconTextureMap = Salad::TextureManager::get().loadTexture2D("assets/textures/file_explorer_icons_32.png");

		std::string path = "assets/";
		scopeToFolder(path);

		updateIgnoredExtensions();

		EditorSettings::pushGroup("File Explorer");
		EditorSettings::pushString("Ignored Extensions", &m_IgnoredExtensions);
		EditorSettings::pushBool("Ignore Unknown Files", &m_IgnoreUnknownFiles);
		EditorSettings::popGroup();
	}

	void FileExplorerPanel::scopeToFolder(std::string& path, bool pushPrev) {
		if(!m_CurrentDirectory.empty() && pushPrev) m_PreviousDirectories.push(m_CurrentDirectory);

		updateIgnoredExtensions();

		m_CurrentDirectory = path;
		for(auto item : m_Items) {
			delete item;
		}
		m_Items.clear();

		m_Selected = -1;

		uint32_t index = 1;
		for (auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			FileExplorerItem* item = new FileExplorerItem(index++);

			item->path = entry.path().string();
			item->filename = FileUtil::fileNameFromPath(item->path);

			if (entry.is_directory()) {
				item->type = FileExplorerItemType::Folder;
			}
			else {
				std::string extension;
				bool hasExtension = FileUtil::fileExtensionFromName(item->filename, &extension);
				if (hasExtension) {
					if (Helper::shouldIgnoreExtension(m_ExtensionIgnoreList, extension)) { delete item; continue; }
					item->type = Helper::itemTypeFromExtension(extension);
				}
				else {
					item->type = FileExplorerItemType::Unknown;
				}
			}

			m_Items.push_back(item);
		}

		if (!m_Items.empty()) {

			// Default sorting: descending based on type
			std::sort(m_Items.begin(), m_Items.end(), [](const FileExplorerItem* a, const FileExplorerItem* b) -> bool {
				return static_cast<int>(a->type) < static_cast<int>(b->type);
			});
		}
	}

	void FileExplorerPanel::scopeToPrevious() {
		if (m_PreviousDirectories.empty()) return;

		std::string path = m_PreviousDirectories.top();
		m_PreviousDirectories.pop();
		scopeToFolder(path, false);
	}

	void FileExplorerPanel::scopeToParent() {
		std::string parentDir = FileUtil::popDirectory(m_CurrentDirectory);
		if(parentDir != m_CurrentDirectory) {
			std::string s = parentDir.append("/");
			scopeToFolder(s);
		}
	}

	void FileExplorerPanel::onItemDoubleClicked(FileExplorerItem* item) {
		switch(item->type) {
			case FileExplorerItemType::GLSL: {
				Asset::AssetSerializer serializer;
				Asset::ShaderAsset& shader = serializer.deserializeShader(item->path);
				EditorSelectionContext::setSelectionContext<ShaderSelectionContext>(shader);
			} break;

			case FileExplorerItemType::Texture: {
				Asset::AssetSerializer serializer;
				Asset::TextureAsset texture = serializer.deserializeTexture(item->path);
				EditorSelectionContext::setSelectionContext<TextureSelectionContext>(texture);
			} break;
		}
	}

	void FileExplorerPanel::refresh() {
		scopeToFolder(m_CurrentDirectory, false);
	}

	void FileExplorerPanel::updateIgnoredExtensions() {

		if (m_PreviousIgnoredExtensions == m_IgnoredExtensions) return;

		m_ExtensionIgnoreList.clear();

		std::string extension = "";
		for(int i = 0; i < m_IgnoredExtensions.size(); i++) {
			char c = m_IgnoredExtensions[i];
			if(c == ' ') {
				if (extension.size() == 0) continue;
				m_ExtensionIgnoreList.push_back(extension);
				extension = "";
			}
			else {
				extension += c;
			}
		}

		if (extension.size() > 0) m_ExtensionIgnoreList.push_back(extension);

		m_PreviousIgnoredExtensions = m_IgnoredExtensions;
	}

	void FileExplorerPanel::onImGuiRender(uint32_t textureid) {

		ImGui::Begin("File Explorer");

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 2.0f, 4.0f });
		//if(ImGui::Button("<", ImVec2(24.0f, 24.0f))) {
		ImGui::PushID(0);
		if (ImGui::ImageButton((void*)textureid, ImVec2{ 16.0f, 16.0f }, ImVec2(0.0f, 0.125f), ImVec2(0.0625f, 0.1875f), 4)) {
			scopeToPrevious();
		}
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::PushID(1);
		if (ImGui::ImageButton((void*)textureid, ImVec2{ 16.0f, 16.0f }, ImVec2(0.0625f, 0.125f), ImVec2(0.125f, 0.1875f), 4)) {
			scopeToParent();
		}
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::PushID(2);
		if (ImGui::ImageButton((void*)textureid, ImVec2{ 16.0f, 16.0f }, ImVec2(0.0f, 0.1875f), ImVec2(0.0625f, 0.25f), 4)) {
			refresh();
		}
		ImGui::PopID();
		ImGui::PopStyleVar();
		ImGui::SameLine();
		ImGui::Text(m_CurrentDirectory.c_str());

		ImGui::Separator();

		int height = ImGui::GetContentRegionAvail().y;

		ImGuiWindowFlags window_flags = 0;
		ImGui::BeginChild("FileExplorerFileTree", ImVec2( 0, height), false, window_flags);

		for (int i = 0; i < m_Items.size(); i++) {

			ImGui::PushID(i);
			FileExplorerItem* item = m_Items[i];

			ImVec2 p = ImGui::GetCursorScreenPos();
			bool selected = m_Selected == item->fileExplorerViewId;
			if (ImGui::Selectable("##fileexplorerselectable", &selected, 0, ImVec2(0.0f, 40.0f))) {
				m_Selected = item->fileExplorerViewId;
			}

			if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
				if(item->type == FileExplorerItemType::Folder) {
					scopeToFolder(item->path.append("/"));
					ImGui::PopID();
					break;
				}
				else {
					onItemDoubleClicked(item);
				}
			}

			ImVec2 afterScreenPos = ImGui::GetCursorScreenPos();

			ImVec2 iconSize = ImVec2{ 32.0f, 32.0f };
			std::pair<float, float> texCoords = Helper::textureCoordsForType(item->type);
			ImGui::GetWindowDrawList()->AddImage((void*)m_FileIconTextureMap->getRendererId(), ImVec2(p.x + 4, p.y + 4), ImVec2(p.x + 36, p.y + 36), 
				ImVec2(texCoords.first, texCoords.second), ImVec2(texCoords.first + 0.125f, texCoords.second + 0.125f));

			ImGui::SetCursorScreenPos(ImVec2(p.x + 40, p.y + 10));
			ImGui::TextUnformatted(item->filename.c_str());
			ImGui::SetCursorScreenPos(afterScreenPos);

			ImGui::PopID();
		}

		ImGui::EndChild();
		ImGui::End();
	}

}
