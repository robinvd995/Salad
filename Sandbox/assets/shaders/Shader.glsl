#version 450 core

struct NestedStruct {
	int bar;
	float foo;
};

struct PointLight {
	vec4 position;
	vec3 color;
	float range;
	int attenuation;
	NestedStruct nested;
};

#ifdef STAGE_VERTEX

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

layout(std140, binding = 0) uniform Scene {
	mat4 u_Projection;
	mat4 u_View;
};

layout(std140, binding = 1) uniform Material {
	float u_Float;
	vec2 u_Vec2;
	vec3 u_Vec3;
	vec4 u_Color;

	int u_Int;
	ivec2 u_Int2;
	ivec3 u_Int3;
	ivec4 u_Int4;

	uint u_UInt;
	uvec2 u_UInt2;

	PointLight u_Light[1][1];

	int u_PostStruct;
};

layout(std140, binding = 2) uniform Transform {
	mat4 u_Transform;
};

layout(location = 0) out vec4 v_Color;

void main() {
	gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);
	v_Color = u_Color;
	#ifdef COMPILE_VULKAN
	ggdhgfhtgf vulkan = VULKANPOG;
	#endif
	#ifdef COMPILE_OPENGL
	float opengl = OPENGLWOOO;
	#endif
	#ifdef COMPILE_DIRECTX
	float directx = CLASSICDIRECTX;
	#endif
	#ifdef COMPILE_METAL
	float metal = METAL;
	#endif

}
#endif

#ifdef STAGE_FRAGMENT

layout(location = 0) in vec4 v_Color;

layout(location = 0) out vec4 frag_color;

layout(binding = 0) uniform sampler1D u_Sampler1D;
layout(binding = 1) uniform sampler1DArray u_Sampler1DArray;
layout(binding = 2) uniform sampler1DShadow u_Sampler1DShadow;
layout(binding = 3) uniform sampler1DArrayShadow u_Sampler1DArrayShadow;

layout(binding = 4) uniform sampler2D u_Sampler2D;
layout(binding = 5) uniform sampler2DArray u_Sampler2DArray;
layout(binding = 6) uniform sampler2DShadow u_Sampler2DShadow;
layout(binding = 7) uniform sampler2DArrayShadow u_Sampler2DArrayShadow;
layout(binding = 8) uniform sampler2DMS u_Sampler2DMS;
layout(binding = 9) uniform sampler2DMSArray u_Sampler2DMSArray;
layout(binding = 10) uniform sampler2DRect u_Sampler2DRect;
layout(binding = 11) uniform sampler2DRectShadow u_Sampler2DRectShadow;

layout(binding = 12) uniform sampler3D u_Sampler3D;
//layout(binding = 13) uniform sampler3DRect u_Sampler3DRect;

layout(binding = 13) uniform samplerBuffer u_SamplerBuffer;
layout(binding = 14) uniform samplerCube u_SamplerCube;
layout(binding = 15) uniform samplerCubeArray u_SamplerCubeArray;
layout(binding = 16) uniform samplerCubeArrayShadow u_SamplerCubeArrayShadow;
layout(binding = 17) uniform samplerCubeShadow u_SamplerCubeShadow;

void main() {
	frag_color = v_Color + vec4(0.0, 0.0, 0.0, 1.0);
}

#endif
