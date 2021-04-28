#include "EditorAssetManager.h"

#include "Assets/AssetManagerSerializer.hpp"

#include <iostream>

namespace Salad {

	EditorAssetManager::EditorAssetManager(const std::string& assetOutput, const std::string& assetRegistry) :
		m_AssetManager(assetOutput, assetRegistry)
	{
		Asset::AssetManagerSerializer serializer;
		serializer.deserialize(m_AssetManager);

		// TODO: Setup fileobserver thread etc.
	}

	EditorAssetManager::~EditorAssetManager() {
		Asset::AssetManagerSerializer serializer;
		serializer.serialize(m_AssetManager);

		// TODO: Destroy fileobserver thread etc.
	}
}
