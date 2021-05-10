// Framebuffer composer shader

$type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

void main() {
	v_TexCoord = a_TexCoord;
	gl_Position = vec4(a_Position, 1.0);
}

$type fragment
#version 330 core

layout(location = 0) out vec4 frag_color;

in vec2 v_TexCoord;

uniform sampler2D u_BaseTexture;
uniform sampler2D u_OverlayTexture;

void main() {
	vec4 baseColor = texture(u_BaseTexture, v_TexCoord);
	vec4 overlayColor = texture(u_OverlayTexture, v_TexCoord);

	if(overlayColor.x == 0) 
		frag_color = baseColor;
	else
		frag_color = overlayColor;
}
