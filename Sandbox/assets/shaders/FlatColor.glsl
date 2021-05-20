#version 450 core

#ifdef STAGE_VERTEX

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

layout(std140, binding = 0) uniform u_Scene {
	mat4 u_Projection;
	mat4 u_View;
};

layout(std140, binding = 1) uniform u_Material {
	vec4 u_Color;
};

layout(std140, binding = 2) uniform Transform {
	mat4 u_Transform;
};

layout(location = 0) out vec4 v_Color;
layout(location = 1) out vec2 v_TexCoord;

void main() {
	gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);
	v_Color = u_Color;
	v_TexCoord = a_TexCoord;
}

#endif

#ifdef STAGE_FRAGMENT

layout(location = 0) in vec4 v_Color;
layout(location = 1) in vec2 v_TexCoord;

layout(location = 0) out vec4 frag_color;

layout(binding = 0) uniform sampler2D u_DiffuseMap;

void main() {
	frag_color = texture(u_DiffuseMap, v_TexCoord) * v_Color;
}

#endif
