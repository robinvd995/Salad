#version 450 core

layout(std140, binding = 0) uniform u_Scene {
	mat4 u_Projection;
	mat4 u_View;
	vec3 u_ViewPosition;
};

#ifdef STAGE_VERTEX

layout(location = 0) in vec3 a_Position;

layout(std140, binding = 2) uniform Transform {
	mat4 u_Transform;
};

layout(location = 0) out vec3 v_Position;

void main() {
	v_Position = a_Position;

    mat4 noTranslateView = u_View;
    noTranslateView[3][0] = 0.0;
    noTranslateView[3][1] = 0.0;
    noTranslateView[3][2] = 0.0;
	gl_Position = u_Projection * noTranslateView * u_Transform * vec4(a_Position, 1.0);
}

#endif

#ifdef STAGE_FRAGMENT

layout(location = 0) in vec3 v_Position;

layout(location = 0) out vec4 frag_color;

layout(binding = 0) uniform sampler2D u_Texture;

void main() {
	vec2 textureCoords = (v_Position.xy + vec2(1.0)) * vec2(0.5);
	vec4 texColor = texture(u_Texture, textureCoords);

	frag_color = texColor;
}

#endif
