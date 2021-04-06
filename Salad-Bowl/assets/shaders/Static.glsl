// 3D Shader

$type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

$add_uniform uniform mat4 u_Projection;
$add_uniform uniform mat4 u_View;
$add_uniform uniform mat4 u_Transform;

void main() {
	gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);
}

$type fragment
#version 330 core

$add_uniform uniform vec3 u_StaticColor;

out vec4 frag_color;

void main() {
	frag_color = vec4(u_StaticColor, 1.0);
}
