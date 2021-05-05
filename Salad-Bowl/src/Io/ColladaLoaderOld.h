#pragma once

#include "Mesh.h"

#include "Salad/Core/Log.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <glm/glm.hpp>

namespace Salad {
	
	class ColladaLoaderOld {

	public:

		ColladaLoaderOld() = default;
		~ColladaLoaderOld() = default;

		std::vector<Mesh*> loadColladaModel(const std::string& filepath);

	};
}