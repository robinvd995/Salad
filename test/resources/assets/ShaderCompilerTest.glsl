#version 450 core

#ifdef STAGE_VERTEX

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

layout(std140, binding = 0) uniform Scene {
    mat4 u_Projection;
    mat4 u_View;
};

layout(std140, binding = 1) uniform Transform {
    mat4 u_Transform;
};

layout(location = 0) out vec2 v_TexCoord;
layout(location = 1) out vec3 v_Normal;
layout(location = 2) out vec3 v_FragPos;

void main() {
    v_TexCoord = a_TexCoord;
    v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
    v_FragPos = vec3(u_Transform * vec4(a_Position, 1.0));
    gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);
}

#endif

#ifdef STAGE_FRAGMENT

layout(std140, binding = 2) uniform Material {
    vec3 u_Ambient;
    vec3 u_Diffuse;
    vec3 u_Specular;
    float u_Shininess;
};

layout(location = 0) out vec4 a_FragColor;

layout(location = 0) in vec2 v_TexCoord;
layout(location = 1) in vec3 v_Normal;
layout(location = 2) in vec3 v_FragPos;

layout(binding = 0) uniform sampler2D u_DiffuseMap;

void main() {
    a_FragColor = vec4(u_Ambient, 1.0);
}

#endif
