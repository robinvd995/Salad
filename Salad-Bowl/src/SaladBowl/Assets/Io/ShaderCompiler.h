#pragma once

#include "Salad/Util/DataBuffers.hpp"

#include <string>
#include <vector>
#include <map>

namespace Salad::Asset {

	typedef uint32_t ShaderCompilerFlags;

	constexpr ShaderCompilerFlags ShaderCompilerFlags_None = 0 << 0;
	constexpr ShaderCompilerFlags ShaderCompilerFlags_CompileOpenGL = 1 << 1;
	constexpr ShaderCompilerFlags ShaderCompilerFlags_CompileDirectX = 1 << 2;
	constexpr ShaderCompilerFlags ShaderCompilerFlags_CompileVulkan = 1 << 3;
	constexpr ShaderCompilerFlags ShaderCompilerFlags_CompileMetal = 1 << 4;
	constexpr ShaderCompilerFlags ShaderCompilerFlags_Reflect = 1 << 5;
	constexpr ShaderCompilerFlags ShaderCompilerFlags_Performance = 1 << 6;

	using SPIRVSource = const std::vector<uint32_t>;
	using SPIRVStageMap = std::map<uint32_t, SPIRVSource>;

	typedef std::map<uint32_t, std::string> OpenGLSourceMap;
	typedef uint32_t VulkanSourceMap;
	typedef std::map<uint32_t, std::string> DirectXSourceMap;
	typedef uint32_t MetalSourceMap;

	class GraphicsAPI {

	public:
		static const size_t SIZE;
		static const GraphicsAPI* VALUES[];

		static const GraphicsAPI OPENGL;
		static const GraphicsAPI DIRECTX;
		static const GraphicsAPI VULKAN;
		static const GraphicsAPI METAL;

	private:
		const uint32_t c_Ordinal;
		const std::string c_Name;

		const std::string c_LanguageSpecificMacro;
		const ShaderCompilerFlags c_CompilerFlag;

	private:
		GraphicsAPI() = delete;
		GraphicsAPI(const uint32_t ordinal, const std::string& name, const std::string macro, const ShaderCompilerFlags compilerFlag);

		GraphicsAPI(const GraphicsAPI&) = delete;
		~GraphicsAPI() = default;

	public:
		const uint32_t ordinal() const;
		const std::string& name() const;

		const std::string& macro() const;
		const ShaderCompilerFlags compilerFlag() const;

	public:
		static const GraphicsAPI* getFromCompilerFlag(ShaderCompilerFlags compilerFlag);
	};

	class ShaderDataType {

	public:
		static const ShaderDataType* VALUES[];

		static const ShaderDataType BOOL;
		static const ShaderDataType INT;
		static const ShaderDataType UINT;
		static const ShaderDataType FLOAT;
		static const ShaderDataType DOUBLE;

		static const ShaderDataType BOOL2, BOOL3, BOOL4;
		static const ShaderDataType INT2, INT3, INT4;
		static const ShaderDataType UINT2, UINT3, UINT4;
		static const ShaderDataType FLOAT2, FLOAT3, FLOAT4;
		static const ShaderDataType DOUBLE2, DOUBLE3, DOUBLE4;

		static const ShaderDataType MAT2, MAT3, MAT4;

		static const ShaderDataType STRUCT;

	private:
		// Member data, all constants
		const uint32_t c_Ordinal;
		const std::string c_Name;

		const uint32_t c_SpirvTypeId;
		const uint32_t c_Rows, c_Columns;
		const uint32_t c_BaseSize;

	private:
		ShaderDataType() = delete;
		ShaderDataType(const uint32_t ordinal, const std::string& name, const uint32_t spirvTypeId,
			const uint32_t rows, const uint32_t columns, const uint32_t baseSize);

		ShaderDataType(const ShaderDataType&) = delete;
		~ShaderDataType() = default;

	public:
		// Member functions
		const uint32_t ordinal() const;
		const std::string& name() const;

		const uint32_t baseSize() const;
		const uint32_t size() const;
		const uint32_t components() const;

	public:
		static const ShaderDataType* getDataType(const uint32_t spirvDataTypeId, uint32_t rows, uint32_t columns);
	};

	class SamplerType {

	public:
		static const SamplerType* VALUES[];

		static const SamplerType SAMPLER_1D, SAMPLER_1D_ARRAY, SAMPLER_1D_SHADOW, SAMPLER_1D_ARRAY_SHADOW;
		static const SamplerType SAMPLER_2D, SAMPLER_2D_ARRAY, SAMPLER_2D_SHADOW, SAMPLER_2D_ARRAY_SHADOW;
		static const SamplerType SAMPLER_2D_MS, SAMPLER_2D_MS_ARRAY, SAMPLER_2D_RECT, SAMPLER_2D_RECT_SHADOW;
		static const SamplerType SAMPLER_3D, SAMPLER_BUFFER;
		static const SamplerType SAMPLER_CUBE, SAMPLER_CUBE_ARRAY, SAMPLER_CUBE_SHADOW, SAMPLER_CUBE_ARRAY_SHADOW;

	private:
		// Member data, all constants
		const uint32_t c_Ordinal;
		const std::string c_Name;

		const uint32_t c_Dimensions;
		const bool c_Depth;
		const bool c_Arrayed;
		const bool c_Multisampled;

	private:
		SamplerType() = delete;
		SamplerType(const uint32_t ordinal, const std::string& name, const uint32_t dimensions, const bool depth, const bool arrayed,
			const bool multisampled);

		SamplerType(const SamplerType&) = delete;
		~SamplerType() = default;

	public:
		const uint32_t ordinal() const;
		const std::string& name() const;

		const uint32_t dimensions() const;
		const bool depth() const;
		const bool arrayed() const;
		const bool multisampled() const;

	public:
		static const SamplerType* getSampler(const uint32_t dimensions, const bool depth, const bool arrayed, const bool multisampled);
	};

	class ShaderStage {

	public:
		static const ShaderStage* VALUES[];

		static const ShaderStage VERTEX;
		static const ShaderStage GEOMETRY;
		static const ShaderStage FRAGMENT;
		static const ShaderStage COMPUTE;

	private:
		const uint32_t c_Ordinal;
		const std::string c_Name;
		const std::string c_StageMacroDefinition;
		const uint32_t c_ShaderKind;

	private:
		ShaderStage() = delete;
		ShaderStage(const uint32_t ordinal, const std::string& name, const std::string& stageMacro, uint32_t shaderKind);
		ShaderStage(const ShaderStage&) = delete;
		~ShaderStage() = default;
	public:
		const uint32_t ordinal() const;
		const std::string macroDefinition() const;
		const uint32_t shaderKind() const;
	};

	class SamplerEntry {

	public:
		SamplerEntry() = delete;
		SamplerEntry(const uint32_t index, const uint32_t binding, const std::string& name, const SamplerType* sampler);
		SamplerEntry(const SamplerEntry&) = delete;
		~SamplerEntry() = default;

		const uint32_t index() const;
		const uint32_t binding() const;
		const std::string& name() const;
		const SamplerType* type() const;

	private:
		const uint32_t c_Index;
		const uint32_t c_Binding;
		const std::string c_Name;
		const SamplerType* c_SamplerType;
	};

	class UniformEntry {

	public:
		UniformEntry() = delete;
		UniformEntry(const UniformEntry&) = delete;
		UniformEntry(const uint32_t index, const std::string& name, const ShaderDataType* dataType);

		void setArray(uint32_t arrayDimensions, std::vector<uint32_t>& arraySizes);
		void setStruct(uint32_t memberCount, uint32_t structSize);

		void addMember(UniformEntry* memberVar);

		void cleanup();

	private:
		const uint32_t c_Index;
		const std::string c_Name;
		const ShaderDataType* c_DataType;

		bool m_IsArray = false;
		uint32_t m_ArrayDimensions = 0;
		std::vector<uint32_t> m_ArraySizes;
		bool m_IsStruct = false;
		uint32_t m_StructCount = 0;
		uint32_t m_StructSize = 0;

		std::vector<UniformEntry*> m_MemberVariables;
	};

	class UniformBuffer {

	public:
		UniformBuffer(const std::string& name, uint32_t size, uint32_t binding, uint32_t memberCount);
		~UniformBuffer();

		const std::string& getName() const;
		const uint32_t getSize() const;
		const uint32_t getBinding() const;
		const uint32_t getMemberCount() const;

		void cleanup();

	private:
		//void addMember(UniformEntry* entry);

	private:
		const std::string c_Name;
		const uint32_t c_Size;
		const uint32_t c_Binding;
		const uint32_t c_MemberCount;

		UniformEntry* m_Root;

		friend class ShaderCompiler;
	};

	class ShaderReflectionData {

	public:
		ShaderReflectionData() = default;
		~ShaderReflectionData() = default;

		const std::vector<UniformBuffer*>& getUniformBuffers();
		const std::vector<SamplerEntry*>& getSamplers();

		UniformBuffer* getMaterialBuffer();
		UniformBuffer* bufferForBinding(uint32_t binding);

	private:
		std::vector<UniformBuffer*> m_UniformBuffers;
		std::vector<SamplerEntry*> m_Samplers;

		friend class ShaderCompiler;
	};

	struct ShaderErrorData {
		const std::string c_Prefix;

		uint32_t m_ErrorCount;
		std::string m_ErrorMessages;

		ShaderErrorData() = delete;
		ShaderErrorData(const std::string& prefix);

		void addErrorMessage(const std::string& error);
	};

	struct ShaderAPICompilerContainer {
		SPIRVStageMap m_SourceMap;
		ShaderErrorData* m_CompilerErrors;
	};

	class ShaderCompiler {

	public:

		ShaderCompiler(const std::string& filename, ShaderCompilerFlags flags);
		~ShaderCompiler();

		bool hasReflectionData();
		ShaderReflectionData* getReflectionData();

		// Error feedback functions
		bool hasCompilerErrors(ShaderCompilerFlags apiCompileFlag);
		const std::string& getCompilerErrorMessages(ShaderCompilerFlags apiCompileFlag);
		bool hasReflectionErrors();
		const std::string& getReflectionErrorMessages();

		void cleanup();

	private:
		void compile();
		//bool compileStage(const std::string& source, const ShaderStage* stage, shaderc::CompileOptions options, ShaderAPICompilerContainer* container);
		void reflectStage(const ShaderStage* stage, SPIRVSource& source);
		//void processStruct(spirv_cross::Compiler& compiler, const spirv_cross::SPIRType& structType, UniformEntry* uniformEntry);

	private:
		const std::string c_Filename;
		const ShaderCompilerFlags c_CompilerFlags;

		//shaderc::Compiler m_Compiler;
		std::vector<const ShaderStage*> m_DeclaredStages;

		std::map<uint32_t, ShaderAPICompilerContainer*> m_ApiCompilerContainers;

		ShaderErrorData* m_ReflectionErrors = nullptr;
		ShaderReflectionData* m_ReflectionData = nullptr;

		friend class ShaderConverter;
	};

	class ShaderConverter {

	public:

		ShaderConverter() = delete;
		ShaderConverter(ShaderCompiler* compiler);
		~ShaderConverter() = default;

		const OpenGLSourceMap getOpenGL() const;
		const VulkanSourceMap getVulkan() const;
		const DirectXSourceMap getDirectX() const;
		const MetalSourceMap getMetal() const;

		void cleanup();

	private:
		ShaderCompiler* m_Compiler;

	};

	class ShaderExporter {
	
	public:
		ShaderExporter() = delete;
		ShaderExporter(const ShaderConverter* converter);
		~ShaderExporter() = default;

		Util::ByteBuffer* buffer();

		void cleanup();

	private:
		void exportShader();

	private:
		const ShaderConverter* c_Converter;
		Util::ByteBuffer* m_Buffer;
	};
}
