#pragma once

#include "Salad/Core/Core.h"

#include "Assets/ModelAsset.hpp"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <string>

namespace Salad::Asset {
	
	class ColladaLoader {

	public:

		ColladaLoader() = default;
		~ColladaLoader() = default;

		void loadColladaModel(const std::string& filepath, Ref<ModelAsset> model);

	private:
		ModelMesh* processMesh(aiMesh* aimesh);
		void addMeshToMap(Ref<ModelAsset> model, ModelMeshNode* node, ModelMesh* mesh, uint32_t index);
		void processNode(aiNode* node, const aiScene* scene, ModelMeshNode* meshNode, Ref<ModelAsset> model);
	};
}