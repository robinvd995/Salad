#include "ShaderCompiler.h"

#include <vulkan/vulkan.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include <spirv_cross/spirv_hlsl.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>

#include <fstream>

#include <iostream>

#include <chrono>

namespace Salad::Asset {

    // ----- Timer -----
    namespace Internal::Timer {

        struct TimerValues {
            std::chrono::steady_clock::time_point startTime;
            std::chrono::steady_clock::time_point endTime;
        } values;

        void start() {
            values.startTime = std::chrono::high_resolution_clock::now();
        }

        void end() {
            values.endTime = std::chrono::high_resolution_clock::now();
        }

        float getTimeMillis() {
            std::chrono::duration<float, std::milli> ms_float = values.endTime - values.startTime;
            return ms_float.count();
        }
    }
    // ----- Timer -----

    // ----- GraphicsAPI -----

    const size_t GraphicsAPI::SIZE = 4;
    const GraphicsAPI* GraphicsAPI::VALUES[SIZE] = {};

    const GraphicsAPI GraphicsAPI::OPENGL(0, "OpenGL", "COMPILE_OPENGL", ShaderCompilerFlags_CompileOpenGL);
    const GraphicsAPI GraphicsAPI::DIRECTX(1, "DirectX", "COMPILE_DIRECTX", ShaderCompilerFlags_CompileDirectX);
    const GraphicsAPI GraphicsAPI::VULKAN(2, "Vulkan", "COMPILE_VULKAN", ShaderCompilerFlags_CompileVulkan);
    const GraphicsAPI GraphicsAPI::METAL(3, "Metal", "COMPILE_METAL", ShaderCompilerFlags_CompileMetal);

    GraphicsAPI::GraphicsAPI(const uint32_t ordinal, const std::string& name, const std::string macro, const ShaderCompilerFlags compilerFlag) :
        c_Ordinal(ordinal), c_Name(name), c_LanguageSpecificMacro(macro), c_CompilerFlag(compilerFlag) {
        VALUES[ordinal] = this;
    }

    const uint32_t GraphicsAPI::ordinal() const {
        return c_Ordinal;
    }

    const std::string& GraphicsAPI::name() const {
        return c_Name;
    }

    const std::string& GraphicsAPI::macro() const {
        return c_LanguageSpecificMacro;
    }

    const ShaderCompilerFlags GraphicsAPI::compilerFlag() const {
        return c_CompilerFlag;
    }

    const GraphicsAPI* GraphicsAPI::getFromCompilerFlag(ShaderCompilerFlags compilerFlag) {
        for (const GraphicsAPI* api : VALUES)
            if (api->compilerFlag() == compilerFlag) return api;
        return nullptr;
    }

    // ----- GraphicsAPI -----

    // ----- Shader Data Type -----

    const ShaderDataType* ShaderDataType::VALUES[24] = {};

    const ShaderDataType ShaderDataType::BOOL(0, "Bool", spirv_cross::SPIRType::Boolean, 1, 1, 4);
    const ShaderDataType ShaderDataType::INT(1, "Int", spirv_cross::SPIRType::Int, 1, 1, 4);
    const ShaderDataType ShaderDataType::UINT(2, "UInt", spirv_cross::SPIRType::UInt, 1, 1, 4);
    const ShaderDataType ShaderDataType::FLOAT(3, "Float", spirv_cross::SPIRType::Float, 1, 1, 4);
    const ShaderDataType ShaderDataType::DOUBLE(4, "Double", spirv_cross::SPIRType::Double, 1, 1, 8);

    const ShaderDataType ShaderDataType::BOOL2(5, "Bool2", ShaderDataType::BOOL.c_SpirvTypeId, 2, 1, ShaderDataType::BOOL.c_BaseSize);
    const ShaderDataType ShaderDataType::BOOL3(6, "Bool3", ShaderDataType::BOOL.c_SpirvTypeId, 3, 1, ShaderDataType::BOOL.c_BaseSize);
    const ShaderDataType ShaderDataType::BOOL4(7, "Bool4", ShaderDataType::BOOL.c_SpirvTypeId, 4, 1, ShaderDataType::BOOL.c_BaseSize);

    const ShaderDataType ShaderDataType::INT2(8, "Int2", ShaderDataType::INT.c_SpirvTypeId, 2, 1, ShaderDataType::INT.c_BaseSize);
    const ShaderDataType ShaderDataType::INT3(9, "Int3", ShaderDataType::INT.c_SpirvTypeId, 3, 1, ShaderDataType::INT.c_BaseSize);
    const ShaderDataType ShaderDataType::INT4(10, "Int4", ShaderDataType::INT.c_SpirvTypeId, 4, 1, ShaderDataType::INT.c_BaseSize);

    const ShaderDataType ShaderDataType::UINT2(11, "UInt2", ShaderDataType::UINT.c_SpirvTypeId, 2, 1, ShaderDataType::UINT.c_BaseSize);
    const ShaderDataType ShaderDataType::UINT3(12, "UInt3", ShaderDataType::UINT.c_SpirvTypeId, 3, 1, ShaderDataType::UINT.c_BaseSize);
    const ShaderDataType ShaderDataType::UINT4(13, "UInt4", ShaderDataType::UINT.c_SpirvTypeId, 4, 1, ShaderDataType::UINT.c_BaseSize);

    const ShaderDataType ShaderDataType::FLOAT2(14, "Float2", ShaderDataType::FLOAT.c_SpirvTypeId, 2, 1, ShaderDataType::FLOAT.c_BaseSize);
    const ShaderDataType ShaderDataType::FLOAT3(15, "Float3", ShaderDataType::FLOAT.c_SpirvTypeId, 3, 1, ShaderDataType::FLOAT.c_BaseSize);
    const ShaderDataType ShaderDataType::FLOAT4(16, "Float4", ShaderDataType::FLOAT.c_SpirvTypeId, 4, 1, ShaderDataType::FLOAT.c_BaseSize);

    const ShaderDataType ShaderDataType::DOUBLE2(17, "Double2", ShaderDataType::DOUBLE.c_SpirvTypeId, 2, 1, ShaderDataType::DOUBLE.c_BaseSize);
    const ShaderDataType ShaderDataType::DOUBLE3(18, "Double3", ShaderDataType::DOUBLE.c_SpirvTypeId, 3, 1, ShaderDataType::DOUBLE.c_BaseSize);
    const ShaderDataType ShaderDataType::DOUBLE4(19, "Double4", ShaderDataType::DOUBLE.c_SpirvTypeId, 4, 1, ShaderDataType::DOUBLE.c_BaseSize);

    const ShaderDataType ShaderDataType::MAT2(20, "Mat2", ShaderDataType::FLOAT.c_SpirvTypeId, 2, 2, ShaderDataType::FLOAT.c_BaseSize);
    const ShaderDataType ShaderDataType::MAT3(21, "Mat3", ShaderDataType::FLOAT.c_SpirvTypeId, 3, 3, ShaderDataType::FLOAT.c_BaseSize);
    const ShaderDataType ShaderDataType::MAT4(22, "Mat4", ShaderDataType::FLOAT.c_SpirvTypeId, 4, 4, ShaderDataType::FLOAT.c_BaseSize);

    const ShaderDataType ShaderDataType::STRUCT(23, "Struct", spirv_cross::SPIRType::Struct, 1, 1, 0);

    ShaderDataType::ShaderDataType(const uint32_t ordinal, const std::string& name, const uint32_t spirvTypeId,
        const uint32_t rows, const uint32_t columns, const uint32_t baseSize) :
        c_Ordinal(ordinal), c_Name(name), c_SpirvTypeId(spirvTypeId),
        c_Rows(rows), c_Columns(columns), c_BaseSize(baseSize) {
        VALUES[ordinal] = this;
    }

    const uint32_t ShaderDataType::ordinal() const {
        return c_Ordinal;
    }

    const std::string& ShaderDataType::name() const {
        return c_Name;
    }

    const uint32_t ShaderDataType::baseSize() const {
        return c_BaseSize;
    }

    const uint32_t ShaderDataType::size() const {
        return c_BaseSize * c_Rows * c_Columns;
    }

    const uint32_t ShaderDataType::components() const {
        return c_Rows * c_Columns;
    }

    const ShaderDataType* ShaderDataType::getDataType(const uint32_t spirvDataTypeId, uint32_t rows, uint32_t columns) {

        for (const ShaderDataType* type : VALUES) {
            if (type->c_SpirvTypeId != spirvDataTypeId) continue;
            if (type->c_Columns != columns) continue;
            if (type->c_Rows != rows) continue;
            return type;
        }

        return nullptr;
    }

    // ----- Shader Data Type -----

    // ----- Shader Sampler -----

    const SamplerType* SamplerType::VALUES[18] = {};

    const SamplerType SamplerType::SAMPLER_1D(0, "Sampler1D", 0, false, false, false);
    const SamplerType SamplerType::SAMPLER_1D_ARRAY(1, "Sampler1DArray", 0, false, true, false);
    const SamplerType SamplerType::SAMPLER_1D_SHADOW(2, "Sampler1DShadow", 0, true, false, false);
    const SamplerType SamplerType::SAMPLER_1D_ARRAY_SHADOW(3, "Sampler1DArrayShadow", 0, true, true, false);

    const SamplerType SamplerType::SAMPLER_2D(4, "Sampler2D", 1, false, false, false);
    const SamplerType SamplerType::SAMPLER_2D_ARRAY(5, "Sampler2DArray", 1, false, true, false);
    const SamplerType SamplerType::SAMPLER_2D_SHADOW(6, "Sampler2DShadow", 1, true, false, false);
    const SamplerType SamplerType::SAMPLER_2D_ARRAY_SHADOW(7, "Sampler2DArrayShadow", 1, true, true, false);

    const SamplerType SamplerType::SAMPLER_2D_MS(8, "Sampler2DMS", 1, false, false, true);
    const SamplerType SamplerType::SAMPLER_2D_MS_ARRAY(9, "Sampler2DMSArray", 1, false, true, true);
    const SamplerType SamplerType::SAMPLER_2D_RECT(10, "Sampler2DRect", 4, false, false, false);
    const SamplerType SamplerType::SAMPLER_2D_RECT_SHADOW(11, "Sampler2DRectShadow", 4, true, false, false);

    const SamplerType SamplerType::SAMPLER_3D(12, "Sampler3D", 2, false, false, false);
    const SamplerType SamplerType::SAMPLER_BUFFER(13, "SamplerBuffer", 5, false, false, false);

    const SamplerType SamplerType::SAMPLER_CUBE(14, "SamplerCube", 3, false, false, false);
    const SamplerType SamplerType::SAMPLER_CUBE_ARRAY(15, "SamplerCubeArray", 3, false, true, false);
    const SamplerType SamplerType::SAMPLER_CUBE_SHADOW(16, "SamplerCubeShadow", 3, true, false, false);
    const SamplerType SamplerType::SAMPLER_CUBE_ARRAY_SHADOW(17, "SamplerCubeArrayShadow", 3, true, true, false);

    SamplerType::SamplerType(const uint32_t ordinal, const std::string& name, const uint32_t dimensions, const bool depth, const bool arrayed, const bool multisampled) :
        c_Ordinal(ordinal), c_Name(name), c_Dimensions(dimensions), c_Depth(depth), c_Arrayed(arrayed), c_Multisampled(multisampled) {
        VALUES[ordinal] = this;
    }

    const uint32_t SamplerType::ordinal() const {
        return c_Ordinal;
    }

    const std::string& SamplerType::name() const {
        return c_Name;
    }

    const uint32_t SamplerType::dimensions() const {
        return c_Dimensions;
    }

    const bool SamplerType::depth() const {
        return c_Depth;
    }

    const bool SamplerType::arrayed() const {
        return c_Arrayed;
    }

    const bool SamplerType::multisampled() const {
        return c_Multisampled;
    }

    const SamplerType* SamplerType::getSampler(const uint32_t dimensions, const bool depth, const bool arrayed, const bool multisampled) {
        for (const SamplerType* sampler : VALUES) {
            if (sampler->c_Dimensions != dimensions) continue;
            if (sampler->c_Depth != depth) continue;
            if (sampler->c_Arrayed != arrayed) continue;
            if (sampler->c_Multisampled != multisampled) continue;
            return sampler;
        }
        return nullptr;
    }

    // ----- Shader Sampler -----

    // ----- Shader Stage -----

    const ShaderStage* ShaderStage::VALUES[4] = {};
    const ShaderStage ShaderStage::VERTEX(0, "Vertex Shader", "STAGE_VERTEX", shaderc_shader_kind::shaderc_vertex_shader);
    const ShaderStage ShaderStage::GEOMETRY(1, "Geometry Shader", "STAGE_GEOMETRY", shaderc_shader_kind::shaderc_geometry_shader);
    const ShaderStage ShaderStage::FRAGMENT(2, "Fragment Shader", "STAGE_FRAGMENT", shaderc_shader_kind::shaderc_fragment_shader);
    const ShaderStage ShaderStage::COMPUTE(3, "Compute Shader", "STAGE_COMPUTE", shaderc_shader_kind::shaderc_compute_shader);

    ShaderStage::ShaderStage(const uint32_t ordinal, const std::string& name, const std::string& stageMacro, uint32_t shaderKind) :
        c_Ordinal(ordinal), c_Name(name), c_StageMacroDefinition(stageMacro), c_ShaderKind(shaderKind) {
        VALUES[ordinal] = this;
    }

    const uint32_t ShaderStage::ordinal() const {
        return c_Ordinal;
    }

    const std::string ShaderStage::macroDefinition() const {
        return c_StageMacroDefinition;
    }

    const uint32_t ShaderStage::shaderKind() const {
        return c_ShaderKind;
    }

    // ----- Shader Stage -----

    // ----- Sampler Entry -----

    SamplerEntry::SamplerEntry(const uint32_t index, const uint32_t binding, const std::string& name, const SamplerType* sampler) :
        c_Index(index), c_Binding(binding), c_Name(name), c_SamplerType(sampler) {

    }

    const uint32_t SamplerEntry::index() const {
        return c_Index;
    }

    const uint32_t SamplerEntry::binding() const {
        return c_Binding;
    }

    const std::string& SamplerEntry::name() const {
        return c_Name;
    }

    const SamplerType* SamplerEntry::type() const {
        return c_SamplerType;
    }

    // ----- Sampler Entry -----

    // ----- Uniform Entry -----

    UniformEntry::UniformEntry(const uint32_t index, const std::string& name, const ShaderDataType* dataType) :
        c_Index(index), c_Name(name), c_DataType(dataType) {
    }

    void UniformEntry::setArray(uint32_t arrayDimensions, std::vector<uint32_t>& arraySizes) {
        m_IsArray = true;
        m_ArrayDimensions = arrayDimensions;
        m_ArraySizes = arraySizes;
    }

    void UniformEntry::setStruct(uint32_t memberCount, uint32_t structSize) {
        m_IsStruct = true;
        m_StructCount = memberCount;
        m_StructSize = structSize;
        m_MemberVariables.resize(memberCount);
    }

    void UniformEntry::addMember(UniformEntry* memberVar) {
        m_MemberVariables[memberVar->c_Index] = memberVar;
    }

    void UniformEntry::cleanup() {
        for (UniformEntry* member : m_MemberVariables) {
            member->cleanup();
            delete member;
        }
    }

    // ----- Uniform Entry -----

    // ----- Uniform Buffer -----

    UniformBuffer::UniformBuffer(const std::string& name, uint32_t size, uint32_t binding, uint32_t memberCount) :
        c_Name(name), c_Size(size), c_Binding(binding), c_MemberCount(memberCount) {
        m_Root = new UniformEntry(0, name, &ShaderDataType::STRUCT);
        m_Root->setStruct(memberCount, size);
    }

    UniformBuffer::~UniformBuffer() {

    }

    const std::string& UniformBuffer::getName() const {
        return c_Name;
    }

    const uint32_t UniformBuffer::getSize() const {
        return c_Size;
    }

    const uint32_t UniformBuffer::getBinding() const {
        return c_Binding;
    }

    const uint32_t UniformBuffer::getMemberCount() const {
        return c_MemberCount;
    }

    void UniformBuffer::cleanup() {
        m_Root->cleanup();
        delete m_Root;
    }

    // ----- Uniform Buffer -----

    // ----- ShaderErrorData -----

    ShaderErrorData::ShaderErrorData(const std::string& prefix) :
        c_Prefix(prefix), m_ErrorCount(0) {
    }

    void ShaderErrorData::addErrorMessage(const std::string& error) {
        m_ErrorMessages.append(c_Prefix).append(error).append("\n");
        m_ErrorCount++;
    }

    // ----- ShaderErrorData -----

    // ----- ShaderReflectionData -----

    const std::vector<UniformBuffer*>& ShaderReflectionData::getUniformBuffers() {
        return m_UniformBuffers;
    }

    const std::vector<SamplerEntry*>& ShaderReflectionData::getSamplers() {
        return m_Samplers;
    }

    // ----- ShaderReflectionData -----

    // ----- Shader Compiler -----

    ShaderCompiler::ShaderCompiler(const std::string& filename, ShaderCompilerFlags flags) :
        c_Filename(filename), c_CompilerFlags(flags) {
        compile();
        //for(int i = 0; i < 23; i++) std::cout << "ShaderDataType[" << i << "]: " << ShaderDataType::VALUES[i]->ordinal() << ", " << ShaderDataType::VALUES[i]->name() << std::endl;
    }

    ShaderCompiler::~ShaderCompiler() {}

    bool ShaderCompiler::hasCompilerErrors(ShaderCompilerFlags apiCompileFlag) {
        const GraphicsAPI* api = GraphicsAPI::getFromCompilerFlag(apiCompileFlag);
        if (!api) return false;
        ShaderAPICompilerContainer* container = m_ApiCompilerContainers[api->ordinal()];
        if (!container) return false;
        return container->m_CompilerErrors->m_ErrorCount > 0;
    }

    const std::string& ShaderCompiler::getCompilerErrorMessages(ShaderCompilerFlags apiCompileFlag) {
        const GraphicsAPI* api = GraphicsAPI::getFromCompilerFlag(apiCompileFlag);
        ShaderAPICompilerContainer* container = m_ApiCompilerContainers[api->ordinal()];
        return container->m_CompilerErrors->m_ErrorMessages;
    }

    bool ShaderCompiler::hasReflectionErrors() {
        return m_ReflectionErrors->m_ErrorCount > 0;
    }

    const std::string& ShaderCompiler::getReflectionErrorMessages() {
        return m_ReflectionErrors->m_ErrorMessages;
    }

    bool compileStage(shaderc::Compiler& compiler, const std::string& source, const ShaderStage* stage, const std::string& filename,
        shaderc::CompileOptions options, ShaderAPICompilerContainer* container) {
        options.AddMacroDefinition(stage->macroDefinition());

        shaderc::SpvCompilationResult compilationResult = compiler.CompileGlslToSpv(source, static_cast<shaderc_shader_kind>(stage->shaderKind()), filename.c_str(), options);
        container->m_CompilerErrors->m_ErrorCount += compilationResult.GetNumErrors();
        if (compilationResult.GetNumErrors() > 0) {
            container->m_CompilerErrors->m_ErrorMessages.append(compilationResult.GetErrorMessage());
            return false;
        }

        const auto& insertResult = container->m_SourceMap.insert({ stage->ordinal(), std::vector<uint32_t>(compilationResult.begin(), compilationResult.end()) });
        return compilationResult.GetCompilationStatus() == shaderc_compilation_status_success;
    }


    void processStruct(spirv_cross::Compiler& compiler, const spirv_cross::SPIRType& structType, UniformEntry* uniformEntry, ShaderErrorData* reflectionErrors) {
        uint32_t structSize = compiler.get_declared_struct_size(structType);
        uint32_t memberCount = structType.member_types.size();
        uniformEntry->setStruct(memberCount, structSize);

        for (int i = 0; i < memberCount; i++) {
            auto& memberType = compiler.get_type(structType.member_types[i]); // The type of the member
            auto& memberName = compiler.get_member_name(structType.self, i); // The name of the member
            uint32_t vecsize = memberType.vecsize; // The amount of rows this type has
            uint32_t colsize = memberType.columns; // The amount of columns this type has

            const ShaderDataType* shaderDataType = ShaderDataType::getDataType(memberType.basetype, vecsize, colsize);
            if (shaderDataType == nullptr) {
                std::string error = "Unable to find a shaderDataType for " + memberName + ", skipping!";
                reflectionErrors->addErrorMessage(error);
                continue;
            }

            UniformEntry* memberEntry = new UniformEntry(i, memberName, shaderDataType);

            if (!memberType.array.empty()) {
                uint32_t arrayDimensions = memberType.array.size();
                std::vector<uint32_t> arraySizes(arrayDimensions);
                for (int i = 0; i < arrayDimensions; i++) arraySizes[i] = memberType.array[i];

                memberEntry->setArray(arrayDimensions, arraySizes);
            }

            bool isStruct = false;
            if (memberType.basetype == spirv_cross::SPIRType::Struct) {
                processStruct(compiler, memberType, memberEntry, reflectionErrors);
                isStruct = true;
            }

            uniformEntry->addMember(memberEntry);
        }
    }

    void ShaderCompiler::compile() {

        // Loading the file contents
        std::ifstream file(c_Filename.c_str());
        std::string source((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
        file.close();

        // Find shader stage declarations
        for (const ShaderStage* stage : ShaderStage::VALUES) {
            std::string stageDeclaration = std::string("#ifdef ").append(stage->macroDefinition());
            size_t pos = source.find(stageDeclaration);
            if (pos == source.npos) continue;
            m_DeclaredStages.push_back(stage);
        }

        // Create compiler
        shaderc::Compiler compiler;

        for (const GraphicsAPI* api : GraphicsAPI::VALUES) {
            // Check if the api is user defined to be compiled, vulkan gets always compiled because of it beeing used to be reflected upon
            if (!(c_CompilerFlags & api->compilerFlag()) && api->ordinal() != GraphicsAPI::VULKAN.ordinal()) continue;

            ShaderAPICompilerContainer* container = new ShaderAPICompilerContainer();

            // Setting up the compiler options to use
            shaderc::CompileOptions options;
            options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
            options.AddMacroDefinition(api->macro());
            if (c_CompilerFlags & ShaderCompilerFlags_Performance)
                options.SetOptimizationLevel(shaderc_optimization_level_performance);

            // Iterating trough the different stages and compiling them if found
            std::string errorPrefix = api->name() + ": compile error: ";
            container->m_CompilerErrors = new ShaderErrorData(errorPrefix);
            for (const ShaderStage* stage : m_DeclaredStages) {
                Internal::Timer::start();
                bool success = compileStage(compiler, source, stage, c_Filename, options, container);
                Internal::Timer::end();
                if (!success) container->m_CompilerErrors->addErrorMessage("Failed to compile stage " + stage->macroDefinition());
                std::cout << "CompileTime: " << Internal::Timer::getTimeMillis() << "ms" << std::endl;
            }

            // TODO: stage verification, vertex and fragment stage need to be present if not a raytraced program

            m_ApiCompilerContainers.insert({ api->ordinal(), container });
        }

        // Reflection
        if (c_CompilerFlags & ShaderCompilerFlags_Reflect) {

            ShaderAPICompilerContainer* vulkanContainer = m_ApiCompilerContainers[GraphicsAPI::VULKAN.ordinal()];

            m_ReflectionData = new ShaderReflectionData();
            m_ReflectionErrors = new ShaderErrorData("Reflection Error: ");

            for (auto it = vulkanContainer->m_SourceMap.begin(); it != vulkanContainer->m_SourceMap.end(); it++) {
                Internal::Timer::start();
                reflectStage(ShaderStage::VALUES[it->first], it->second);
                Internal::Timer::end();
                std::cout << "ReflectTime: " << Internal::Timer::getTimeMillis() << "ms" << std::endl;
            }
        }
    }

    void ShaderCompiler::reflectStage(const ShaderStage* stage, SPIRVSource& source) {

        spirv_cross::Compiler compiler(source);
        spirv_cross::ShaderResources resources = compiler.get_shader_resources();

        for (const auto& resource : resources.uniform_buffers) {
            const spirv_cross::SPIRType& type = compiler.get_type(resource.base_type_id);
            uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            uint32_t bufferSize = compiler.get_declared_struct_size(type);
            uint32_t memberCount = type.member_types.size();

            UniformBuffer* uniformBuffer = new UniformBuffer(resource.name, bufferSize, binding, memberCount);
            processStruct(compiler, type, uniformBuffer->m_Root, m_ReflectionErrors);
            m_ReflectionData->m_UniformBuffers.push_back(uniformBuffer);
        }

        int samplerIndex = 0;
        for (const auto& resource : resources.sampled_images) {
            auto& type = compiler.get_type(resource.type_id);
            uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);

            const SamplerType* sampler = SamplerType::getSampler(type.image.dim, type.image.depth, type.image.arrayed, type.image.ms);
            if (sampler == nullptr) {
                std::string error = "Unable to find a SamplerType for " + resource.name + ", skipping!";
                m_ReflectionErrors->addErrorMessage(error);
                continue;
            }

            SamplerEntry* samplerEntry = new SamplerEntry(samplerIndex++, binding, resource.name, sampler);
            m_ReflectionData->m_Samplers.push_back(samplerEntry);
        }
    }

    void ShaderCompiler::cleanup() {
        m_DeclaredStages.clear();
        for (auto it = m_ApiCompilerContainers.begin(); it != m_ApiCompilerContainers.end(); it++) {
            it->second->m_SourceMap.clear();
            delete it->second;
        }

        if (m_ReflectionData) {

            if(!m_ReflectionData->m_UniformBuffers.empty())
                for (UniformBuffer* ub : m_ReflectionData->m_UniformBuffers) {
                    ub->cleanup();
                    delete ub;
                }

            if (!m_ReflectionData->m_Samplers.empty())
                for (SamplerEntry* sampler : m_ReflectionData->m_Samplers) {
                    delete sampler;
                }
        }
    }

    bool ShaderCompiler::hasReflectionData() {
        return m_ReflectionData != nullptr;
    }

    ShaderReflectionData* ShaderCompiler::getReflectionData() {
        return m_ReflectionData;
    }

    // ----- Shader Compiler -----
}