#include "gtest/gtest.h"

#include <Salad.h>

#include "Salad/Util/Archiver.hpp"
#include "SaladBowl/Assets/Core/AssetManager.h"
#include "SaladBowl/Assets/Core/AssetManagerSerializer.hpp"

#include "Salad/Core/ResourceManager.h"
#include "Salad/Util/DataBuffers.hpp"

#include "SaladBowl/Assets/Io/ColladaLoader.h"
#include "SaladBowl/Assets/Io/ShaderCompiler.h"

#include <vector>
#include <iostream>

namespace Salad {

	TEST(AssetManager_Tests, Generic) {
		using namespace Salad::Asset;

		const std::string resourcePath = "resources.zip";
		const std::string assetManagerPath = "assetmanagertest";
		{ // Write
			AssetManager manager(resourcePath, assetManagerPath);
			manager.clean();

			manager.includeAsset("assets/textures/checkerboard.png", AssetType::Texture);
			manager.includeAsset("assets/textures/crate_diffuse.png", AssetType::Texture);
			manager.includeAsset("assets/textures/crate_specular.png", AssetType::Texture);
			manager.includeAsset("assets/shaders/test/ShaderBuilderTest.glsl", AssetType::Shader);

			AssetManagerSerializer serializer;
			serializer.serialize(manager);
		} // Write

		{ // Read
			AssetManager manager(resourcePath, assetManagerPath);
			AssetManagerSerializer serializer;
			serializer.deserialize(manager);

			manager.buildAll(false);

			serializer.serialize(manager);

		} // Read
	}
}