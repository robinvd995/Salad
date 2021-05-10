#include "gtest/gtest.h"

#include "SaladBowl/Assets/Io/ShaderCompiler.h"

namespace Salad {

	TEST(ShaderCompiler_Tests, Generic) {
		using namespace Salad::Asset;
		ShaderCompilerFlags flags = ShaderCompilerFlags_CompileOpenGL | ShaderCompilerFlags_CompileVulkan | ShaderCompilerFlags_Reflect;
		ShaderCompiler compiler("assets/shaders/test/ShaderCompilerTest.glsl", flags);

		if (compiler.hasCompilerErrors(ShaderCompilerFlags_CompileOpenGL))
			std::cout << compiler.getCompilerErrorMessages(ShaderCompilerFlags_CompileOpenGL) << std::endl;
		else std::cout << "OpenGL compilation success!" << std::endl;

		if (compiler.hasCompilerErrors(ShaderCompilerFlags_CompileVulkan))
			std::cout << compiler.getCompilerErrorMessages(ShaderCompilerFlags_CompileVulkan) << std::endl;
		else std::cout << "Vulkan compilation success!" << std::endl;

		if (compiler.hasReflectionData()) {
			ShaderReflectionData* reflectionData = compiler.getReflectionData();

			const auto& uniformBuffers = reflectionData->getUniformBuffers();
			for (auto uniformBuffer : uniformBuffers) {
				std::cout << "UniformBufferName: " << uniformBuffer->getName() << std::endl;
			}
		}

		compiler.cleanup();
	}
}
