// Texture Shader

$type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

$add_uniform uniform mat4 u_ViewProjection;
$add_uniform uniform mat4 u_Transform;
$add_uniform uniform vec2 u_TexMapSize;
$add_uniform uniform vec2 u_TexMapCoords;


out vec2 v_TexCoord;

void main() {
	v_TexCoord = a_TexCoord * u_TexMapSize + u_TexMapCoords;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

$type fragment
#version 330 core

out vec4 frag_color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main() {
	frag_color = texture(u_Texture, v_TexCoord) * u_Color;
}