#pragma once

#include "Core/AssetBase.h"
#include "ShaderAsset.hpp"

#include <vector>

namespace Salad::Asset {

	struct TextureVariable {
		uint32_t textureUnit = 0;
		std::string name = "";
		std::string resourceId = "";
	};

	struct UniformVariable {
		std::string name = "";
		ShaderAssetDataType type = ShaderAssetDataType::None;
		void* value = nullptr;
	};

	class MaterialAsset : public AssetBase {
	
	public:
		MaterialAsset() = default;
		~MaterialAsset() = default;

		virtual bool hasAssetFile() override { return false; };
		virtual AssetType getAssetType() override { return AssetType::Material; }
		virtual uint64_t calculateAssetSize() override { return (uint64_t)0; }

		void setFileName(const std::string& filename) { m_FileName = filename; }
		const std::string& getFileName() { return m_FileName; }

		void reflectFromShader(Ref<ShaderAsset> shader) {
			m_UniformVariables.clear();
			m_TextureVariables.clear();

			for(int i = 0; i < shaderStagesSize; i++) {
				ShaderStageType stageType = shaderStages[i];
				if (!shader->hasStage(stageType)) continue;
				ShaderStage stage = shader->getStage(stageType);
				
				for(auto& variable : stage.uniforms) {
					ShaderAssetDataType t = variable.type;
					if(t == ShaderAssetDataType::Sampler2D) {
						TextureVariable texture;
						texture.name = variable.identifier;
						texture.textureUnit = variable.layoutLocation;
						m_TextureVariables.push_back(texture);
					}
					else {
						UniformVariable uniform;
						uniform.name = variable.identifier;
						uniform.type = variable.type;
						m_UniformVariables.push_back(uniform);
					}
				}
			}
		}

		std::vector<UniformVariable>& getUniforms() { return m_UniformVariables; }
		std::vector<TextureVariable>& getTextures() { return m_TextureVariables; }

		void setShaderAssetId(const std::string shader) { m_ShaderAssetId = shader; }
		const std::string& getShaderAssetId() { return m_ShaderAssetId; }

	private:
		std::string m_FileName;

		std::string m_ShaderAssetId = "shader";
		std::vector<UniformVariable> m_UniformVariables;
		std::vector<TextureVariable> m_TextureVariables;
	};
}
