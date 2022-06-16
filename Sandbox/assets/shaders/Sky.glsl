#version 450 core

layout(std140, binding = 0) uniform u_Scene {
    mat4 u_Projection;
    mat4 u_View;
    vec3 u_ViewPosition;
};

#ifdef STAGE_VERTEX

layout(location = 0) in vec3 a_Position;

layout(std140, binding = 2) uniform Transform {
    mat4 u_Transform;
};

layout(location = 0) out vec3 v_Position;

void main() {
    v_Position = a_Position;
    mat4 noTranslateView = u_View;
    noTranslateView[3][0] = 0.0;
    noTranslateView[3][1] = 0.0;
    noTranslateView[3][2] = 0.0;
    gl_Position = u_Projection * noTranslateView * vec4(a_Position, 1.0);
}

#endif

#ifdef STAGE_FRAGMENT

layout(location = 0) in vec3 v_Position;

layout(location = 0) out vec4 frag_color;

layout(std140, binding = 3) uniform Lights {
    vec3 u_DirectionalLight;
    vec3 u_LightPos;
};

layout(std140, binding = 4) uniform u_Time {
    float u_Timer;
    float u_NormalizedTimer;
    float u_TimeOfDay;
};

layout(std140, binding = 5) uniform Specular {
    vec3 u_SpecularColor;
    float u_Specular;
    float u_Shininess;
};

void main() {
    vec3 dawnColor = vec3(0.97, 0.5, 0.07);
    vec3 skyColor = vec3(0.44, 0.78, 1.0);
    vec3 duskColor = vec3(0.72, 0.41, 0.56);
    vec3 nightColor = vec3(0.0, 0.0, 0.0);

    float weight0 = max(0.0, -pow(((u_TimeOfDay + 1) * 4), 2) + 1);
    float weight1 = max(0.0, -pow(((u_TimeOfDay + 0.5) * 2), 8) + 1);
    float weight2 = max(0.0, -pow(u_TimeOfDay * 4, 2) + 1);
    float weight3 = max(0.0, -pow(((u_TimeOfDay - 0.5) * 2), 8) + 1);
    float weight4 = max(0.0, -pow(((u_TimeOfDay - 1) * 4), 2) + 1);

    vec3 baseColor = nightColor;
    baseColor += mix(nightColor, dawnColor, weight0);
    baseColor += mix(nightColor, skyColor, weight1);
    baseColor += mix(nightColor, duskColor, weight2);
    baseColor += mix(nightColor, dawnColor, weight4);

    //if(u_TimeOfDay < 0.0) skyColor = vec3(0.0, 0.0, 0.0);

    frag_color = vec4(baseColor, 1.0);
}

#endif
