// Gui Shader

$type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

$add_uniform uniform mat4 u_Projection;
$add_uniform uniform mat4 u_View;
$add_uniform uniform mat4 u_Transform;

$add_uniform uniform vec4 u_Color0;
$add_uniform uniform vec4 u_Color1;
$add_uniform uniform vec4 u_Color2;

out vec4 v_Color;

void main() {
	gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);
	v_Color = vec4((u_Color0 + (u_Color1) + (u_Color2)).xyz, 1.0);
}

$type fragment
#version 330 core

out vec4 frag_color;

in vec4 v_Color;

void main() {
	frag_color = vec4(v_Color.xyz, 1.0);
}
