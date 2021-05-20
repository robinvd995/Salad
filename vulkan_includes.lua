VULKAN_SDK = os.getenv("VULKAN_SDK")

VulkanLib = "%{VULKAN_SDK}/Lib"
VulkanInclude = "%{VULKAN_SDK}/Include"

VulkanLibraries = {}
VulkanDebugLibraries = {}

VulkanLibraries["Vulkan"] = "%{VulkanLib}/vulkan-1.lib"
VulkanLibraries["VulkanUtils"] = "%{VulkanLib}/VkLayer_utils.lib"
VulkanLibraries["ShaderC"] = "%{VulkanLib}/shaderc_shared.lib"
VulkanLibraries["SPIRV_Cross"] = "%{VulkanLib}/spirv-cross-core.lib"
VulkanLibraries["SPIRV_Cross_GLSL"] = "%{VulkanLib}/spirv-cross-glsl.lib"
VulkanLibraries["SPIRV_Cross_HLSL"] = "%{VulkanLib}/spirv-cross-hlsl.lib"

VulkanDebugLibraries["ShaderC_Debug"] = "%{VulkanLib}/shaderc_sharedd.lib"
VulkanDebugLibraries["SPIRV_Cross_Debug"] = "%{VulkanLib}/spirv-cross-cored.lib"
VulkanDebugLibraries["SPIRV_Cross_GLSL_Debug"] = "%{VulkanLib}/spirv-cross-glsld.lib"
VulkanDebugLibraries["SPIRV_Cross_HLSL_Debug"] = "%{VulkanLib}/spirv-cross-hlsld.lib"
VulkanDebugLibraries["SPIRV_Tools_Debug"] = "%{VulkanLib}/SPIRV-Toolsd.lib"
