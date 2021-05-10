#include "FileExplorerPanel.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "Salad/Renderer/TextureManager.h"
#include "SaladBowl/EditorSelectionContext.h"

#include <iostream>
#include <filesystem>
#include <map>
#include <algorithm>

#include "SaladBowl/Util/FileUtils.hpp";

#include "SaladBowl/EditorSettings.hpp"
#include "SaladBowl/Assets/Core/AssetSerializer.h"
#include "SaladBowl/EditorAssetManager.h"

#include "SaladBowl/EditorGui/EditorStyle.h"
#include "SaladBowl/EditorGui/EditorGuiTemplates.h"

namespace Salad::EditorGui {

	namespace Helper {

		// TODO: use the asset type here
		static const std::map<std::string, FileExplorerItemType> s_FileTypeMap{
			{"png", FileExplorerItemType::Texture},
			{"dae", FileExplorerItemType::Model},
			{"glsl", FileExplorerItemType::GLSL},
			{"ttf", FileExplorerItemType::TrueTypeFont},
			{"cs", FileExplorerItemType::SharpScript},
			{"lua", FileExplorerItemType::LuaScript},
			{"mat", FileExplorerItemType::Material}
		};

		static const std::map<FileExplorerItemType, std::pair<float, float>> s_FileTypeIconCoordinateMap{
			{ FileExplorerItemType::Unknown, { 0.5f, 0.0f } },
			{ FileExplorerItemType::Folder, { 0.0f, 0.0f } },
			{ FileExplorerItemType::Model, { 0.125f, 0.0f } },
			{ FileExplorerItemType::Texture, { 0.25f, 0.0f } },
			{ FileExplorerItemType::GLSL, { 0.375f, 0.0f } },
			{ FileExplorerItemType::TrueTypeFont, { 0.625f, 0.0f } },
			{ FileExplorerItemType::SharpScript, { 0.75f, 0.0f } },
			{ FileExplorerItemType::LuaScript, { 0.875f, 0.0f } },
			{ FileExplorerItemType::Material, { 0.125f, 0.125f } }
		};

		static const std::map<Asset::AssetState, std::pair<float, float>> s_AssetIconCoordinateMap{
			{ Asset::AssetState::Unknown, { 0.0625f, 0.1875f } },
			{ Asset::AssetState::UpToDate, { 0.0f, 0.125f } },
			{ Asset::AssetState::Dirty, { 0.0625f, 0.125f } },
			{ Asset::AssetState::NotIncluded, { 0.0f, 0.1875f } }
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

		std::pair<float, float> textureCoordsForAssetState(Asset::AssetState state) {
			auto it = s_AssetIconCoordinateMap.find(state);
			if (it == s_AssetIconCoordinateMap.end()) return { 0.0f, 0.0f };
			return it->second;
		}

		bool shouldIgnoreExtension(std::vector<std::string>& ignoreList, std::string& extension) {
			for (std::string& ignore : ignoreList) {
				if (extension == ignore) return true;
			}
			return false;
		}

		Asset::AssetType assetTypeFromFileType(FileExplorerItemType type) {
			switch(type) {
				default: return Asset::AssetType::Unknown;
				case FileExplorerItemType::Model: return Asset::AssetType::Model;
				case FileExplorerItemType::Texture: return Asset::AssetType::Texture;
				case FileExplorerItemType::GLSL: return Asset::AssetType::Shader;
				case FileExplorerItemType::Material: return Asset::AssetType::Material;
			}
		}

	}

	void FileExplorerPanel::init() {

		std::string path = "assets/"; // TODO Make constexpr or #define
		scopeToFolder(path);
	}

	void FileExplorerPanel::loadSettings() {
		EditorSettings::pushGroup("File Explorer");

		EditorSettings::pushString("Ignored Extensions", &m_IgnoredExtensions);
		EditorSettings::pushBool("Ignore Unknown Files", &m_IgnoreUnknownFiles);

		EditorSettings::pushSubGroup("Asset State Icons");
		EditorSettings::pushBool("Show 'Unknown'", &m_AssetStateIconShouldDraw[static_cast<int>(Asset::AssetState::Unknown)]);
		EditorSettings::pushBool("Show 'UpToDate'", &m_AssetStateIconShouldDraw[static_cast<int>(Asset::AssetState::UpToDate)]);
		EditorSettings::pushBool("Show 'Dirty'", &m_AssetStateIconShouldDraw[static_cast<int>(Asset::AssetState::Dirty)]);
		EditorSettings::pushBool("Show 'Not Included'", &m_AssetStateIconShouldDraw[static_cast<int>(Asset::AssetState::NotIncluded)]);
		EditorSettings::popSubGroup();

		EditorSettings::popGroup();

		updateIgnoredExtensions();
	}

	void FileExplorerPanel::scopeToFolder(std::string& path, bool pushPrev) {
		if (!m_CurrentDirectory.empty() && pushPrev) m_PreviousDirectories.push(m_CurrentDirectory);

		updateIgnoredExtensions();

		m_CurrentDirectory = path;
		for (auto item : m_Items) {
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
		if (parentDir != m_CurrentDirectory) {
			std::string s = parentDir.append("/");
			scopeToFolder(s);
		}
	}

	void FileExplorerPanel::onItemDoubleClicked(FileExplorerItem* item) {
		switch (item->type) {
			case FileExplorerItemType::GLSL:
			{
				Asset::AssetSerializer serializer;
				Asset::ShaderAsset& shader = serializer.deserializeShader(item->path);
				EditorSelectionContext::setSelectionContext<ShaderSelectionContext>(shader);
			} break;

			case FileExplorerItemType::Texture:
			{
				Asset::AssetSerializer serializer;
				Asset::TextureAsset texture = serializer.deserializeTexture(item->path);
				EditorSelectionContext::setSelectionContext<TextureSelectionContext>(texture);
			} break;

			case FileExplorerItemType::Model:
			{
				Asset::AssetSerializer serializer;
				Ref<Asset::ModelAsset> model = serializer.deserializeModel(item->path);
				EditorSelectionContext::setSelectionContext<ModelSelectionContext>(model);
			} break;

			case FileExplorerItemType::Material:
			{
				Asset::AssetSerializer serializer;
				Ref<Asset::MaterialAsset> material = serializer.deserializeMaterial(item->path);
				EditorSelectionContext::setSelectionContext<MaterialSelectionContext>(material);
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
		for (int i = 0; i < m_IgnoredExtensions.size(); i++) {
			char c = m_IgnoredExtensions[i];
			if (c == ' ') {
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

	void FileExplorerPanel::onImGuiRender() {

		uint32_t iconsTextureId = EditorGui::EditorStyle::getEditorIcons()->getRendererId();

		ImGui::Begin("File Explorer");

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 2.0f, 4.0f });
		//if(ImGui::Button("<", ImVec2(24.0f, 24.0f))) {
		ImGui::PushID(0);
		if (ImGui::ImageButton((void*)iconsTextureId, ImVec2{ 16.0f, 16.0f }, ImVec2(0.0f, 0.125f), ImVec2(0.0625f, 0.1875f), 4)) {
			scopeToPrevious();
		}
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::PushID(1);
		if (ImGui::ImageButton((void*)iconsTextureId, ImVec2{ 16.0f, 16.0f }, ImVec2(0.0625f, 0.125f), ImVec2(0.125f, 0.1875f), 4)) {
			scopeToParent();
		}
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::PushID(2);
		if (ImGui::ImageButton((void*)iconsTextureId, ImVec2{ 16.0f, 16.0f }, ImVec2(0.0f, 0.1875f), ImVec2(0.0625f, 0.25f), 4)) {
			refresh();
		}
		ImGui::PopID();
		ImGui::SameLine();

		float filterInputWidth = ImGui::GetContentRegionAvail().x - 24.0f;
		ImGui::PushItemWidth(filterInputWidth);
		if (ImGui::InputText("##Filter", m_StringBuffer, sizeof(m_StringBuffer))) {
			m_Filter = std::string(m_StringBuffer);
		}
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::PushID(3);
		if (ImGui::Button("...", ImVec2{ 24.0f, 24.0f })) {}

		if (ImGui::BeginPopupContextItem("FileExplorerContextPopup", ImGuiMouseButton_Left)) {

			templateContextItem("context_build_all", "Build", ContextItemData().setPrefix(0.1875f, 0.1875f, 0.25f, 0.25f, 16, 16), []() {
				EditorAssetManager::assetManager().buildAll(false);
			});

			templateContextItem("context_rebuild_all", "Rebuild", ContextItemData(), []() {
				EditorAssetManager::assetManager().buildAll(true);
			});

			templateContextItem("context_clean", "Clean", ContextItemData(), []() {
				EditorAssetManager::assetManager().clean();
			});

			ImGui::EndPopup();
		}

		ImGui::PopID();

		ImGui::PopStyleVar();
		ImGui::Text(m_CurrentDirectory.c_str());

		ImGui::Separator();

		int height = ImGui::GetContentRegionAvail().y;

		ImGuiWindowFlags window_flags = 0;
		ImGui::BeginChild("FileExplorerFileTree", ImVec2( 0, height), false, window_flags);

		for (int i = 0; i < m_Items.size(); i++) {

			ImGui::PushID(i);
			FileExplorerItem* item = m_Items[i];
			const bool isAsset = Asset::AssetManager::isAsset(item->path);

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

			Asset::AssetManager& assetManager = EditorAssetManager::assetManager();
			if (ImGui::BeginPopupContextItem(item->path.c_str(), ImGuiMouseButton_Right)) {
				
				templateContextItem("context_open", "Open", ContextItemData().setPrefix(0.0625f, 0.1875f, 0.125f, 0.25f, 16, 16).setAffix("CTRL+O"), [item](){
					std::cout << "Open: " << item->filename << std::endl;
				});
				templateContextItem("context_delete", "Delete", ContextItemData().setPrefix(0.125f, 0.125f, 0.1875f, 0.1875f, 16, 16).setAffix("DEL"), [item]() {
					std::cout << "Delete: " << item->filename << std::endl;
				});

				if (isAsset) {
					ImGui::Separator();

					if(!assetManager.isAssetIncluded(item->path)) {
						templateContextItem("context_include", "Include", ContextItemData().setPrefix(0.1875f, 0.125f, 0.25f, 0.1875f, 16, 16), [&assetManager, item]() {
							assetManager.includeAsset(item->path, Helper::assetTypeFromFileType(item->type));
						});
					}
					else {
						templateContextItem("context_exclude", "Exclude", ContextItemData().setPrefix(0.125f, 0.1875f, 0.1875f, 0.25f, 16, 16), [&assetManager, item]() {
							assetManager.excludeAsset(item->path);
						});
					}

					templateContextItem("context_build", "Build Asset", ContextItemData().setPrefix(0.1875f, 0.1875f, 0.25f, 0.25f, 16, 16), [&assetManager, item]() {
						assetManager.buildAsset(item->path, true);
					});
				}
				ImGui::EndPopup();
			}

			ImVec2 afterScreenPos = ImGui::GetCursorScreenPos();

			ImVec2 iconSize = ImVec2{ 32.0f, 32.0f };
			std::pair<float, float> texCoords = Helper::textureCoordsForType(item->type);
			uint32_t textureId = EditorGui::EditorStyle::getFileIcons()->getRendererId();
			ImGui::GetWindowDrawList()->AddImage((void*)textureId, ImVec2(p.x + 4, p.y + 4), ImVec2(p.x + 36, p.y + 36),
				ImVec2(texCoords.first, texCoords.second), ImVec2(texCoords.first + 0.125f, texCoords.second + 0.125f));

			if (isAsset) {
				Asset::AssetState assetState = assetManager.getAssetState(item->path);
				if (m_AssetStateIconShouldDraw[static_cast<int>(assetState)]) {
					auto stateIconCoords = Helper::textureCoordsForAssetState(assetState);
					ImGui::GetWindowDrawList()->AddImage((void*)textureId, ImVec2(p.x + 20, p.y + 20), ImVec2(p.x + 36, p.y + 36),
						ImVec2(stateIconCoords.first, stateIconCoords.second), ImVec2(stateIconCoords.first + 0.0625f, stateIconCoords.second + 0.0625f));
				}
			}

			ImGui::SetCursorScreenPos(ImVec2(p.x + 40, p.y + 10));
			ImGui::TextUnformatted(item->filename.c_str());
			ImGui::SetCursorScreenPos(afterScreenPos);

			ImGui::PopID();
		}

		ImGui::EndChild();
		ImGui::End();
	}

}