#include "gtest/gtest.h"

#include "Salad/Renderer/Shader.h"

#include "SaladBowl/Assets/Io/ShaderCompiler.h"

#include <fstream>

namespace Salad {

#define SHADER_COMPILER_TEST_FILE "assets/ShaderCompilerTest.glsl"

	TEST(ShaderCompiler_Tests, Generic) {
		using namespace Salad::Asset;
		ShaderCompilerFlags flags = ShaderCompilerFlags_CompileOpenGL | ShaderCompilerFlags_CompileVulkan | ShaderCompilerFlags_CompileDirectX
			| ShaderCompilerFlags_CompileMetal| ShaderCompilerFlags_Reflect;
		ShaderCompiler compiler(SHADER_COMPILER_TEST_FILE, flags);

		ASSERT_FALSE(compiler.hasCompilerErrors(ShaderCompilerFlags_CompileOpenGL));
		ASSERT_FALSE(compiler.hasCompilerErrors(ShaderCompilerFlags_CompileVulkan));
		ASSERT_FALSE(compiler.hasCompilerErrors(ShaderCompilerFlags_CompileDirectX));
		ASSERT_FALSE(compiler.hasCompilerErrors(ShaderCompilerFlags_CompileMetal));

		ASSERT_TRUE(compiler.hasReflectionData());

		compiler.cleanup();
	}

	TEST(ShaderCompiler_Tests, Reflection) {
		using namespace Salad::Asset;
		ShaderCompilerFlags flags = ShaderCompilerFlags_CompileVulkan | ShaderCompilerFlags_Reflect;
		ShaderCompiler compiler(SHADER_COMPILER_TEST_FILE, flags);

		if (compiler.hasCompilerErrors(ShaderCompilerFlags_CompileVulkan)) std::cout << compiler.getCompilerErrorMessages(ShaderCompilerFlags_CompileVulkan) << std::endl;

		ASSERT_FALSE(compiler.hasCompilerErrors(ShaderCompilerFlags_CompileVulkan));
		ASSERT_FALSE(compiler.hasReflectionErrors());
		ASSERT_TRUE(compiler.hasReflectionData());

		ShaderReflectionData* reflectionData = compiler.getReflectionData();
		const auto& uniformbuffers = reflectionData->getUniformBuffers();
		ASSERT_EQ(uniformbuffers.size(), 3);

		UniformBuffer* boundUniformBuffers[3] = {};
		for (auto uniformBuffer : uniformbuffers) {
			boundUniformBuffers[uniformBuffer->getBinding()] = uniformBuffer;
		}

		ASSERT_EQ(boundUniformBuffers[0]->getName(), "Scene");
		ASSERT_EQ(boundUniformBuffers[0]->getMemberCount(), 2);
		ASSERT_EQ(boundUniformBuffers[0]->getSize(), 128);

		ASSERT_EQ(boundUniformBuffers[1]->getName(), "Transform");
		ASSERT_EQ(boundUniformBuffers[1]->getMemberCount(), 1);
		ASSERT_EQ(boundUniformBuffers[1]->getSize(), 64);

		ASSERT_EQ(boundUniformBuffers[2]->getName(), "Material");
		ASSERT_EQ(boundUniformBuffers[2]->getMemberCount(), 4);
		ASSERT_EQ(boundUniformBuffers[2]->getSize(), 48);

		const auto& samplers = reflectionData->getSamplers();
		ASSERT_EQ(samplers.size(), 1);

		ASSERT_EQ(samplers[0]->binding(), 0);
		ASSERT_EQ(samplers[0]->name(), "u_DiffuseMap");

		compiler.cleanup();
	}

	TEST(ShaderCompiler_Tests, OpenGLConversion) {
		using namespace Salad::Asset;
		ShaderCompilerFlags flags = ShaderCompilerFlags_CompileOpenGL;
		ShaderCompiler compiler(SHADER_COMPILER_TEST_FILE, flags);
		ShaderConverter converter(&compiler);
		const OpenGLSourceMap openglSourceMap = converter.getOpenGL();

		ASSERT_EQ(openglSourceMap.size(), 2);

		for (auto it = openglSourceMap.begin(); it != openglSourceMap.end(); it++) {
			std::cout << it->second << std::endl;
		}
	}

	TEST(ShaderCompiler_Tests, DirectXConversion) {
		using namespace Salad::Asset;
		ShaderCompilerFlags flags = ShaderCompilerFlags_CompileDirectX;
		ShaderCompiler compiler(SHADER_COMPILER_TEST_FILE, flags);
		ShaderConverter converter(&compiler);
		const DirectXSourceMap openglSourceMap = converter.getDirectX();

		ASSERT_EQ(openglSourceMap.size(), 2);

		for (auto it = openglSourceMap.begin(); it != openglSourceMap.end(); it++) {
			std::cout << it->second << std::endl;
		}

		ASSERT_EQ(1, 1);
	}

	TEST(ShaderCompiler_Tests, ShaderExport) {
		using namespace Salad::Asset;
		ShaderCompilerFlags flags = ShaderCompilerFlags_CompileOpenGL;
		ShaderCompiler compiler(SHADER_COMPILER_TEST_FILE, flags);

		ShaderConverter converter(&compiler);
		ShaderExporter exporter(&converter);

		Util::ByteBuffer* buffer = exporter.buffer();

		std::ofstream out("ShaderCompiler_Tests-ShaderExport.txt");
		out << *buffer;
		out.close();

		//buffer->moveIterator(0);
		//Ref<Shader> shader = Shader::create("testshader", buffer);

		exporter.cleanup();
		ASSERT_TRUE(true);

		/*buffer->moveIterator(0);

		uint64_t openglindex = buffer->read<uint64_t>();
		uint64_t directxindex = buffer->read<uint64_t>();
		uint64_t vulkanindex = buffer->read<uint64_t>();
		uint64_t metalindex = buffer->read<uint64_t>();

		std::cout << openglindex << "," << directxindex << "," << vulkanindex << "," << metalindex << std::endl;

		std::cout << "ItIndex0: " << buffer->getIteratorIndex() << std::endl;

		if(openglindex > 0) {
			buffer->moveIterator(openglindex);
			std::cout << "ItIndex1: " << buffer->getIteratorIndex() << std::endl;

			uint32_t openglVertexStageSize = buffer->read<uint32_t>();
			if (openglVertexStageSize > 0) {
				char* vertexStage = buffer->readArray<char>(openglVertexStageSize);
				std::string vertexString = std::string(vertexStage);
				std::cout << vertexString << std::endl;
			}

			uint32_t openglGeometryStageSize = buffer->read<uint32_t>();
			if(openglGeometryStageSize > 0) {
				char* geometryStage = buffer->readArray<char>(openglGeometryStageSize);
				std::string geometryString = std::string(geometryStage);
				std::cout << geometryString << std::endl;
			}

			uint32_t openglFragmentStageSize = buffer->read<uint32_t>();
			if(openglFragmentStageSize > 0) {
				char* fragmentStage = buffer->readArray<char>(openglFragmentStageSize);
				std::string fragmentString = std::string(fragmentStage);
				std::cout << fragmentString << std::endl;
			}

			uint32_t openglComputeStageSize = buffer->read<uint32_t>();
			if(openglComputeStageSize > 0) {
				char* computeStage = buffer->readArray<char>(openglComputeStageSize);
				std::string computeString = std::string(computeStage);
				std::cout << computeString << std::endl;
			}
		}*/
	}

	TEST(ShaderCompiler_Tests, ShaderExportFunc) {
		using namespace Salad::Asset;
		ShaderCompilerFlags flags = ShaderCompilerFlags_CompileOpenGL;
		ShaderCompiler compiler("assets/ShaderCompilerFlatColor.glsl", flags);

		ShaderConverter converter(&compiler);
		ShaderExporter exporter(&converter);

		Util::ByteBuffer* buffer = exporter.buffer();

		std::ofstream out("ShaderCompiler_Tests-ShaderExportFunc.txt");
		out << *buffer;
		out.close();

		exporter.cleanup();
		ASSERT_TRUE(true);
	}
}
