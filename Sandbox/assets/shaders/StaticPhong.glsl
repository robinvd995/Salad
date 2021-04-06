// 3D Specular Shader

$type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

$add_uniform uniform mat4 u_Projection;
$add_uniform uniform mat4 u_View;
$add_uniform uniform mat4 u_Transform;

out vec2 v_TexCoord;
out vec3 v_Normal;

out vec3 v_FragPos;

void main() {
	v_TexCoord = a_TexCoord;
	// Transpose matrix on cpu and send it to shader, more efficient
	v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;

	v_FragPos = vec3(u_Transform * vec4(a_Position, 1.0));

	gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);
}

$type fragment
#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 frag_color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

$add_uniform uniform vec3 u_ViewPos;
$add_uniform uniform Light u_Light;
$add_uniform uniform Material u_Material;

void main() {
	
	// Ambient Light
	vec3 ambient = u_Light.ambient * u_Material.ambient;

	// Diffuse Light
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(u_Light.position - v_FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_Light.diffuse * (diff * u_Material.diffuse);

	// Specular
	vec3 viewDir = normalize(u_ViewPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
	vec3 specular = u_Light.specular * (spec * u_Material.specular);

	vec3 result = ambient + diffuse + specular;
	frag_color = vec4(result, 1.0);
}
