// Skybox shader

$type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

out vec3 v_TexCoords;

$add_uniform uniform mat4 u_Projection;
$add_uniform uniform mat4 u_View;

void main() {
	v_TexCoords = a_Position;
	gl_Position = u_Projection * u_View * vec4(a_Position, 1.0);
}

$type fragment
#version 330 core

in vec3 v_TexCoords;

uniform samplerCube skybox;

layout(location = 0) out vec4 frag_color;

void main() {
	frag_color = texture(skybox, v_TexCoords);
}
