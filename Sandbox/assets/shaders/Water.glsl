#version 450 core

layout(std140, binding = 1) uniform u_Material {
	vec4 u_BumpProperties0;
	vec4 u_BumpProperties1;
};

layout(std140, binding = 0) uniform u_Scene {
	mat4 u_Projection;
	mat4 u_View;
	vec3 u_ViewPosition;
};

#ifdef STAGE_VERTEX

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in float a_SurfaceDistance;

layout(std140, binding = 2) uniform Transform {
	mat4 u_Transform;
};

layout(std140, binding = 4) uniform u_Time {
	float u_Timer;
	float u_NormalizedTimer;
};

layout(location = 0) out float v_SurfaceDistance;
layout(location = 1) out vec2 v_TextureCoords;
layout(location = 2) out vec3 v_Normal;
layout(location = 3) out vec3 v_FragPosition;

layout(binding = 0) uniform sampler2D u_WaterBump0;
layout(binding = 1) uniform sampler2D u_WaterBump1;
layout(binding = 2) uniform sampler2D u_WaterNormal0;
layout(binding = 3) uniform sampler2D u_WaterNormal1;

void main() {
	v_TextureCoords = vec2(a_Position.x, a_Position.z);

	vec2 sampleCoords0 = v_TextureCoords / u_BumpProperties0.x + u_Timer * u_BumpProperties0.y;
	vec2 sampleCoords1 = -v_TextureCoords / u_BumpProperties1.x + u_Timer * u_BumpProperties1.y;

	float bumpSample0 = texture(u_WaterBump0, sampleCoords0).x * u_BumpProperties0.z - u_BumpProperties0.w;
	float bumpSample1 = texture(u_WaterBump1, sampleCoords1).x * u_BumpProperties1.z - u_BumpProperties1.w;

	vec3 normSample0 = texture(u_WaterNormal0, sampleCoords0).xzy - vec3(0.5, 0.0, 0.5);
	vec3 normSample1 = texture(u_WaterNormal1, sampleCoords1).xzy - vec3(0.5, 0.0, 0.5);

	v_Normal = normalize(normSample0 + normSample1);

	float height = bumpSample0 + bumpSample1;
	vec3 pos = a_Position;
	pos.y += height;

	v_SurfaceDistance = a_SurfaceDistance + height;
	v_FragPosition = vec3(u_Transform * vec4(pos, 1.0));
	gl_Position = u_Projection * u_View * u_Transform * vec4(pos, 1.0);
}

#endif

#ifdef STAGE_FRAGMENT

layout(location = 0) in float v_SurfaceDistance;
layout(location = 1) in vec2 v_TextureCoords;
layout(location = 2) in vec3 v_Normal;
layout(location = 3) in vec3 v_FragPosition;

layout(location = 0) out vec4 frag_color;

layout(std140, binding = 3) uniform Lights {
	vec3 u_DirectionalLight;
};

layout(std140, binding = 5) uniform Specular {
	vec3 u_SpecularColor;
	float u_Specular;
	float u_Shininess;
};

void main() {
	if(v_SurfaceDistance < 0.0) discard;

	float maxSurfaceDistance = 0.8;
	float surfaceDistance = min(v_SurfaceDistance, maxSurfaceDistance) / maxSurfaceDistance;

	vec3 waterColor = mix(vec3(0.2, 0.7, 0.9), vec3(0.2, 0.3, 0.9), surfaceDistance);
	if(surfaceDistance < 0.08) waterColor = vec3(0.9, 0.9, 0.9);

	vec3 ambient = vec3(0.1, 0.1, 0.1);
	vec3 lightDirection = normalize(u_DirectionalLight);
	vec3 norm = normalize(v_Normal);
	
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = (diff + ambient);

	// Specular
	vec3 viewDir = normalize(u_ViewPosition - v_FragPosition);
	vec3 reflectDir = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);
	vec3 specular = u_SpecularColor * (spec * u_Specular);

	vec3 result = (waterColor * diffuse) + specular;
	frag_color = vec4(result, 0.9);
}

#endif
