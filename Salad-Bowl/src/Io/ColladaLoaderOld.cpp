#include "ColladaLoaderOld.h"

#include "Mesh.h"

#include <iostream>

namespace Salad {

	Mesh* processMesh(aiMesh* mesh) {

		Mesh* meshinstance = new Mesh();

		FloatBuffer& vertexBuffer = meshinstance->getBuffer();
		std::vector<uint32_t>& indexBuffer = meshinstance->getIndexBuffer();

		vertexBuffer.resize(mesh->mNumVertices * (3 + 3 + 2));

		for(uint32_t i = 0; i < mesh->mNumVertices; i++) {

			meshinstance->getBuffer().pushFloat3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

			float u, v = 0;
			if(mesh->mTextureCoords[0]) {
				u = mesh->mTextureCoords[0][i].x;
				v = mesh->mTextureCoords[0][i].y;
			}
			meshinstance->getBuffer().pushFloat2(u, v);
		
			meshinstance->getBuffer().pushFloat3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}

		for(uint32_t i = 0; i < mesh->mNumFaces; i++) {
			aiFace& face = mesh->mFaces[i];
			for(uint32_t j = 0; j < face.mNumIndices; j++) {
				indexBuffer.push_back(face.mIndices[j]);
			}
		}

		return meshinstance;
	}

	void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshvector) {

		for(uint32_t i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshvector.push_back(processMesh(mesh));
		}
	
		for(uint32_t i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene, meshvector);
		}
	}

	std::vector<Mesh*> ColladaLoaderOld::loadColladaModel(const std::string& filepath) {
	
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

		SLD_CORE_ASSERT(scene, "Error while loading model {0}!", filepath);

		std::vector<Mesh*> meshes;
		if(scene->HasMeshes()) {
			processNode(scene->mRootNode, scene, meshes);
		}

		return meshes;

	}

}