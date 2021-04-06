// Gui Shader

$type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

$add_uniform uniform mat4 u_ViewProjection;
$add_uniform uniform mat4 u_Transform;
$add_uniform uniform vec4 u_Color;

out vec4 v_Color;

void main() {
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
	v_Color = u_Color;
}

$type fragment
#version 330 core

out vec4 frag_color;

in vec4 v_Color;

void main() {
	frag_color = vec4(v_Color.xyz, 1.0);
}
