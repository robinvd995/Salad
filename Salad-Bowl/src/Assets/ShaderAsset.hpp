#pragma once

#include "Core/AssetBase.h"

#include <set>
#include <string>
#include <map>
#include <vector>

#include <iostream>


namespace Salad::Asset {

	enum class ShaderAssetDataType {
		None,
		Mat2, Mat3, Mat4,
		Vec2, Vec3, Vec4,
		Int, Float,
		Sampler2D
	};

	enum class ShaderDeclarationQualifier {
		None,
		In, Out, Uniform,
		Layout, Flat
	};

	// Order should be sorted in the same order the stage is called in the render pipeline, vertex first, fragment last
	enum class ShaderStageType {
		None = 0,
		Vertex = 1,
		Fragment = 2
	};

	inline constexpr ShaderStageType shaderStages[] = { ShaderStageType::Vertex, ShaderStageType::Fragment };
	inline constexpr uint32_t shaderStagesSize = 2;

	// Struct representation of a shader variable
	struct ShaderVariable {

		std::string identifier;
		ShaderAssetDataType type;

		bool flat = false;
		int layoutLocation = -1;
	};

	struct ShaderStage {

		ShaderStageType shaderType = ShaderStageType::None;
		std::string shaderSource;

		std::vector<ShaderVariable> inputs;
		std::vector<ShaderVariable> outputs;
		std::vector<ShaderVariable> uniforms;

		bool operator < (const ShaderStage& other) const { return static_cast<int>(shaderType) < static_cast<int>(other.shaderType); };
	};

	struct ShaderVertexBufferElement {

		std::string name;
		ShaderAssetDataType type = ShaderAssetDataType::None;
		uint32_t size = 0;
		uint32_t offset = 0;

		ShaderVertexBufferElement() = default;
		ShaderVertexBufferElement(std::string& pname, ShaderAssetDataType ptype) : name(pname), type(ptype) {}

	};

	class ShaderVertexBufferLayout {

	public:
		ShaderVertexBufferLayout() = default;
		~ShaderVertexBufferLayout() = default;

		const std::vector<ShaderVertexBufferElement>& getElements() const { return m_BufferElements; }
		const uint32_t getStride() const { return m_Stride; }

		std::vector<ShaderVertexBufferElement>::iterator begin() { return m_BufferElements.begin(); }
		std::vector<ShaderVertexBufferElement>::iterator end() { return m_BufferElements.end(); }
		std::vector<ShaderVertexBufferElement>::const_iterator begin() const { return m_BufferElements.begin(); }
		std::vector<ShaderVertexBufferElement>::const_iterator end() const { return m_BufferElements.end(); }

	private:
		std::vector<ShaderVertexBufferElement> m_BufferElements;
		uint32_t m_Stride = 0;

		friend class ShaderAssetBuilder;
	};

	class ShaderAsset : public AssetBase {

	public:
		ShaderAsset() = default;
		ShaderAsset(const std::string& filepath) : m_FilePath(filepath) {}
		ShaderAsset(const ShaderAsset&) = default;
		~ShaderAsset() {}

		const std::string& getFilePath() { return m_FilePath; }

		bool hasStage(ShaderStageType type) { return m_ShaderStages.find(type) != m_ShaderStages.end(); }
		ShaderStage& getStage(ShaderStageType type) { /*TODO: Assert*/ return m_ShaderStages.find(type)->second; }

		const ShaderVertexBufferLayout& getVertexBufferLayout() const { return m_VertexBufferLayout; }

		// TODO:
		// bool validateModel(Model model); // validates a given model to check if the model inputs match the shader inputs
		// VertexBufferLayout getVertexBufferLayout(); // gets the layout of the vertex buffer
		// UniformBuffer getUniformBufferLayout(); // gets the layout of the uniform buffer
		// FramebufferSpecification getFramebufferSpecification(); // gets the framebuffer specifications of this shader, what color attachments etc
		// 
		// get a instance of a useable shader by the renderer
		// 
		// probably more

		virtual AssetType getAssetType() override { return AssetType::Shader; }

		virtual uint64_t calculateAssetSize() override { 
			uint64_t assetSize = (uint64_t)(8 + 8 + 4 + 4);

			if (hasStage(ShaderStageType::Vertex)) {
				ShaderStage& stage = getStage(ShaderStageType::Vertex);
				assetSize += stage.shaderSource.size();
				
			}
			if (hasStage(ShaderStageType::Fragment)) {
				ShaderStage& stage = getStage(ShaderStageType::Fragment);
				assetSize += stage.shaderSource.size();
			}
			return assetSize;
		}

	private:
		std::map<ShaderStageType, ShaderStage> m_ShaderStages;
		ShaderVertexBufferLayout m_VertexBufferLayout;

		std::string m_FilePath;

		friend class ShaderAssetBuilder;

	};
}
