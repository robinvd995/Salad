#version 450 core

#ifdef STAGE_VERTEX

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

layout(std140, binding = 0) uniform u_Scene {
	mat4 u_Projection;
	mat4 u_View;
};

layout(std140, binding = 2) uniform Transform {
	mat4 u_Transform;
};

layout(location = 0) flat out vec3 v_Color;
layout(location = 1) out vec2 v_TexCoord;
layout(location = 2) flat out vec3 v_Normal;

void main() {
	
	vec3 pos = a_Position;

	if(a_Position.y < 0.5) {
		//v_Color = vec3(0.2, 0.2, 0.8);
		//pos.y = 0.5;
		v_Color = vec3(0.8, 0.6, 0.2);
	}
	else if(a_Position.y < 0.75) {
		v_Color = vec3(0.8, 0.6, 0.2);
	}
	else if(a_Position.y < 2.5) {
		v_Color = vec3(0.3, 0.7, 0.2);
	}
	else if(a_Position.y < 5.0) {
		v_Color = vec3(0.2, 0.5, 0.1);
	}
	else {
		v_Color = vec3(0.9, 0.9, 1.0);
	}

	gl_Position = u_Projection * u_View * u_Transform * vec4(pos, 1.0);
	v_TexCoord = a_TexCoord;
	v_Normal = a_Normal;
}

#endif

#ifdef STAGE_FRAGMENT

layout(location = 0) flat in vec3 v_Color;
layout(location = 1) in vec2 v_TexCoord;
layout(location = 2) flat in vec3 v_Normal;

layout(std140, binding = 3) uniform Lights {
	vec3 u_DirectionalLight;
};

layout(location = 0) out vec4 frag_color;

void main() {

	vec3 ambient = vec3(0.1, 0.1, 0.1);
	vec3 lightDirection = normalize(u_DirectionalLight);
	vec3 norm = normalize(v_Normal);
	
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = (diff + ambient) * v_Color;

	//frag_color = vec4(v_Normal.x + 0.5, v_Normal.y, v_Normal.z + 0.5, 1.0);
	frag_color = vec4(diffuse.xyz, 1.0);
	//frag_color = vec4(v_LightDir, 1.0);
}

#endif
