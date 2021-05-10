#include "EditorAssetManager.h"

#include "Assets/Core/AssetManagerSerializer.hpp"

#include <iostream>

namespace Salad {

	void EditorAssetManager::instantiate(const std::string& assetOutput, const std::string& assetRegisry) {
		// Set the instance to a new instance
		s_Instance = new EditorAssetManager(assetOutput, assetRegisry);

		// Setup fileobserver callback functions / thread
		s_Instance->m_FileObserver.setFileCreatedCallback(EditorAssetManager::onFileCreated);
		s_Instance->m_FileObserver.setFileChangedCallback(EditorAssetManager::onFileChanged);
		s_Instance->m_FileObserver.setFileDeletedCallback(EditorAssetManager::onFileDeleted);

		s_Instance->m_FileObserverThreadRunning = true;
		s_Instance->m_FileObserverThread = new std::thread(EditorAssetManager::observe);

		// Subscribe to AssetManager include/exlcude
		s_Instance->m_AssetManager.subscribeToInclude(onAssetIncluded);
		s_Instance->m_AssetManager.subscribeToExclude(onAssetExcluded);

		// Deserialize the asset registry
		s_Instance->deserialize();
	}

	EditorAssetManager::EditorAssetManager(const std::string& assetOutput, const std::string& assetRegistry) :
		m_AssetManager(assetOutput, assetRegistry),
		m_FileObserver(1000, true)
	{}

	EditorAssetManager::~EditorAssetManager() {
		//Serialize the asset registry
		serialize();

		// Destroy fileobserver thread
		m_FileObserverThreadRunning = false;
		m_FileObserverThread->join();
		delete m_FileObserverThread;
	}

	void EditorAssetManager::observe() {

		// TODO: make it so that it can be interupted
		while(s_Instance->m_FileObserverThreadRunning) {
			s_Instance->m_FileObserver.observe();
		}

	}

	void EditorAssetManager::onFileCreated(const std::string& filepath) {
		std::cout << "onFileCreated!\n";
	}

	void EditorAssetManager::onFileChanged(const std::string& filepath) {
		std::string sourceFile = filepath;
		
		if(Asset::isAssetFile(filepath)) sourceFile = Asset::assetSourceFromAssetFile(filepath);
		
		s_Instance->m_AssetManager.markAssetDirty(sourceFile);
	}

	void EditorAssetManager::onFileDeleted(const std::string& filepath) {
		if (Asset::isAssetFile(filepath)) return;
		s_Instance->m_AssetManager.excludeAsset(filepath, false);
	}

	void EditorAssetManager::onAssetIncluded(const std::string& filepath) {
		std::string& assetFilepath = Asset::assetFileFromPath(filepath, false);
		s_Instance->m_FileObserver.addFile(filepath);
		s_Instance->m_FileObserver.addFile(assetFilepath);
		s_Instance->serialize();
	}

	void EditorAssetManager::onAssetExcluded(const std::string& filepath) {
		std::string& assetFilepath = Asset::assetFileFromPath(filepath, false);
		s_Instance->m_FileObserver.removeFile(filepath);
		s_Instance->m_FileObserver.removeFile(assetFilepath);
	}

	void EditorAssetManager::serialize() {
		Asset::AssetManagerSerializer serializer;
		serializer.serialize(m_AssetManager);
	}

	void EditorAssetManager::deserialize() {
		Asset::AssetManagerSerializer serializer;
		serializer.deserialize(s_Instance->m_AssetManager);
	}
}
