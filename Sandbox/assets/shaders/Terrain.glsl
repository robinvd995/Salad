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
layout(location = 2) out vec3 v_Normal;
layout(location = 3) out vec3 v_FragPosition;

void main() {
	
	vec3 pos = a_Position;

	if(a_Position.y < 2.2) {
		v_Color = vec3(0.8, 0.75, 0.4);
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
	v_FragPosition = a_Position;
}

#endif

#ifdef STAGE_FRAGMENT

layout(location = 0) flat in vec3 v_Color;
layout(location = 1) in vec2 v_TexCoord;
layout(location = 2) in vec3 v_Normal;
layout(location = 3) in vec3 v_FragPosition;

layout(std140, binding = 3) uniform Lights {
	vec3 u_DirectionalLight;
};

layout(location = 0) out vec4 frag_color;

layout(binding = 0) uniform sampler2D u_SandTexture;
layout(binding = 1) uniform sampler2D u_GrassTexture;
layout(binding = 2) uniform sampler2D u_RockTexture;
layout(binding = 3) uniform sampler2D u_SnowTexture;

void main() {

	//vec3 sampleX = vec3(1.0, 0.0, 0.0);
	//vec3 sampleY = vec3(0.0, 1.0, 0.0);
	//vec3 sampleZ = vec3(0.0, 0.0, 1.0);

	float gradientStart = 2.0;
	float gradientEnd = 3.2;
	float heightBlendFactor = clamp((v_FragPosition.y - gradientStart) / (gradientEnd - gradientStart), 0.0, 1.0);

	float snowBlendStart = 6.0;
	float snowBlendEnd = 8.0;
	float snowBlendFactor = clamp((v_FragPosition.y - snowBlendStart) / (snowBlendEnd - snowBlendStart), 0.0, 1.0);

	vec3 sampleX0 = texture(u_RockTexture, v_FragPosition.zy).xyz;
	vec3 sampleY0 = texture(u_SandTexture, v_FragPosition.xz).xyz;
	vec3 sampleZ0 = texture(u_RockTexture, v_FragPosition.xy).xyz;

	vec3 sampleX1 = texture(u_RockTexture, v_FragPosition.zy).xyz;
	vec3 sampleY1 = texture(u_GrassTexture, v_FragPosition.xz).xyz;
	vec3 sampleZ1 = texture(u_RockTexture, v_FragPosition.xy).xyz;

	vec3 sampleX2 = texture(u_SnowTexture, v_FragPosition.zy).xyz;
	vec3 sampleY2 = texture(u_SnowTexture, v_FragPosition.xz).xyz;
	vec3 sampleZ2 = texture(u_SnowTexture, v_FragPosition.xy).xyz;

	vec3 sampleX = mix(mix(sampleX0, sampleX1, heightBlendFactor), sampleX2, snowBlendFactor);
	vec3 sampleY = mix(mix(sampleY0, sampleY1, heightBlendFactor), sampleY2, snowBlendFactor);
	vec3 sampleZ = mix(mix(sampleZ0, sampleZ1, heightBlendFactor), sampleZ2, snowBlendFactor);

	vec3 weights = abs(v_Normal.xyz);
	weights = weights / (weights.x + weights.y + weights.z);

	vec3 tex = sampleX * weights.x + sampleY * weights.y + sampleZ * weights.z;

	vec3 ambient = vec3(0.1, 0.1, 0.1);
	vec3 lightDirection = normalize(u_DirectionalLight);
	vec3 norm = normalize(v_Normal);
	
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = (diff + ambient) * tex.xyz;

	//frag_color = vec4(v_Normal.x + 0.5, v_Normal.y, v_Normal.z + 0.5, 1.0);
	frag_color = vec4(diffuse.xyz, 1.0);

	//vec3 normalColor = v_Normal + 1.0 / 2.0;
	//frag_color = vec4(normalColor.xzy, 1.0);
}

#endif
