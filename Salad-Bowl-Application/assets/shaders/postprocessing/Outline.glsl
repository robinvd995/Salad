// Framebuffer outline shader

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

$add_uniform uniform vec4 u_OutlineColor;

uniform sampler2D u_Texture;

void main() {

	vec4 texColor = texture(u_Texture, v_TexCoord);
    if(texColor.x == 1.0) discard;

	vec2 tex_offset = 1.0 / textureSize(u_Texture, 0);

	int bordersize = 2;

	float avg = 0.0;

	for(int i = -bordersize; i <= bordersize; i++) {
		for(int j = -bordersize; j <= bordersize; j++) {
			vec2 texcoords = vec2(v_TexCoord.x + tex_offset.x * i, v_TexCoord.y + tex_offset.y * i);
			avg += texture(u_Texture, texcoords).x;
		}
	}

    if(avg > 0)
        frag_color = u_OutlineColor;
    else
	    discard;
}
