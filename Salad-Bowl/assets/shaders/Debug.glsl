// 3D Shader

$type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

$add_uniform uniform mat4 u_Projection;
$add_uniform uniform mat4 u_View;
$add_uniform uniform mat4 u_Transform;

out vec2 v_TexCoord;
out vec3 v_Color;

void main() {
	v_TexCoord = a_TexCoord;
	gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);
}

$type fragment
#version 330 core

out vec4 frag_color;

in vec2 v_TexCoord;
in vec3 v_Color;

uniform sampler2D u_Texture;

void main() {
	vec4 texColor = texture(u_Texture, v_TexCoord);
	frag_color = texColor;
}
