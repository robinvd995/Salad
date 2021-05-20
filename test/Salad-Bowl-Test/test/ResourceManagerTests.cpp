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

	TEST(ResourceManager_Tests, Generic) {
		ResourceHandle textureHandle = ResourceManager::allocateTexture("textures.crate_diffuse");
		ResourceHandle textureHandle2 = ResourceManager::allocateTexture("textures.crate_specular");
		Texture2D* texture = ResourceManager::getTexture(textureHandle);
		Texture2D* texture2 = ResourceManager::getTexture(textureHandle2);
		ResourceManager::freeTexture(textureHandle);

		ResourceHandle textureHandle3 = ResourceManager::allocateTexture("textures.checkerboard");
		Texture2D* texture3 = ResourceManager::getTexture(textureHandle3);
	}

}

