// 2D Shader

$type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

$add_uniform uniform mat4 u_ViewProjection;
$add_uniform uniform mat4 u_Transform;

$add_uniform uniform vec2 u_UVSize;
$add_uniform uniform vec2 u_UVCoords;

out vec2 v_TexCoord;

void main() {
	v_TexCoord = (a_TexCoord * u_UVSize) + u_UVCoords;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

$type fragment
#version 330 core

out vec4 frag_color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main() {
	vec4 texColor = texture(u_Texture, v_TexCoord) * u_Color;
	//vec4 texColor = vec4(1.0, 1.0, 1.0, texture(u_Texture, v_TexCoord).r);
	if(texColor.a == 0)
		discard;

	frag_color = texColor;
}