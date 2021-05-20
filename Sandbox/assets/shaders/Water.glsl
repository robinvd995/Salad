#version 450 core

layout(std140, binding = 1) uniform u_Material {
	vec4 u_BumpProperties0;
	vec4 u_BumpProperties1;
};

#ifdef STAGE_VERTEX

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in float a_SurfaceDistance;

layout(std140, binding = 0) uniform u_Scene {
	mat4 u_Projection;
	mat4 u_View;
};

layout(std140, binding = 2) uniform Transform {
	mat4 u_Transform;
};

layout(std140, binding = 4) uniform u_Time {
	float u_Timer;
	float u_NormalizedTimer;
};

layout(location = 0) out float v_SurfaceDistance;
layout(location = 1) out vec2 v_TextureCoords;

layout(binding = 0) uniform sampler2D u_WaterBump0;
layout(binding = 1) uniform sampler2D u_WaterBump1;

void main() {
	v_TextureCoords = vec2(a_Position.x, a_Position.z);

	//float bumpSample0 = texture(u_WaterBump0, v_TextureCoords / 32.0 + u_Timer * 0.05).x * 0.5 - 0.25;
	//float bumpSample1 = texture(u_WaterBump1, -v_TextureCoords / 64.0 + u_Timer * 0.01).x * 0.25 - 0.125;

	float bumpSample0 = texture(u_WaterBump0, v_TextureCoords / u_BumpProperties0.x + u_Timer * u_BumpProperties0.y).x * u_BumpProperties0.z - u_BumpProperties0.w;
	float bumpSample1 = texture(u_WaterBump1, -v_TextureCoords / u_BumpProperties1.x + u_Timer * u_BumpProperties1.y).x * u_BumpProperties1.z - u_BumpProperties1.w;
	float height = bumpSample0 + bumpSample1;
	vec3 pos = a_Position;
	pos.y += height;
	//frag_color = vec4(texture(u_WaterTexture, v_TextureCoords / 128.0 + u_Timer * 0.25).xyz, 0.8);

	v_SurfaceDistance = a_SurfaceDistance + height;
	gl_Position = u_Projection * u_View * u_Transform * vec4(pos, 1.0);
}

#endif

#ifdef STAGE_FRAGMENT

layout(location = 0) in float v_SurfaceDistance;
layout(location = 1) in vec2 v_TextureCoords;

layout(location = 0) out vec4 frag_color;

void main() {
	if(v_SurfaceDistance < 0.0) discard;

	float maxSurfaceDistance = 0.8;
	float surfaceDistance = min(v_SurfaceDistance, maxSurfaceDistance) / maxSurfaceDistance;

	vec3 waterColor = mix(vec3(0.2, 0.7, 0.9), vec3(0.2, 0.3, 0.9), surfaceDistance);
	if(surfaceDistance < 0.08) waterColor = vec3(0.9, 0.9, 0.9);

	frag_color = vec4(waterColor, 1.0);
	//frag_color = vec4(v_TextureCoords.x / 128.0, v_TextureCoords.y / 128.0, 0.5, 1.0);
	//frag_color = vec4(texture(u_WaterTexture, v_TextureCoords * u_TextureScaling + u_Timer * 0.25).xyz, 0.8);
}

#endif
