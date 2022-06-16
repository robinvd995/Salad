                                ,  #version 450

layout(binding = 0, std140) uniform u_Scene
{
    mat4 u_Projection;
    mat4 u_View;
    vec3 u_ViewPosition;
} _19;

layout(binding = 2, std140) uniform Transform
{
    mat4 u_Transform;
} _55;

layout(location = 0) out vec3 v_Position;
layout(location = 0) in vec3 a_Position;

void main()
{
    v_Position = a_Position;
    mat4 noTranslateView = _19.u_View;
    noTranslateView[3].x = 0.0;
    noTranslateView[3].y = 0.0;
    noTranslateView[3].z = 0.0;
    gl_Position = (_19.u_Projection * noTranslateView) * vec4(a_Position, 1.0);
}

    J  #version 450

layout(binding = 4, std140) uniform u_Time
{
    float u_Timer;
    float u_NormalizedTimer;
    float u_TimeOfDay;
} _31;

layout(binding = 0, std140) uniform u_Scene
{
    mat4 u_Projection;
    mat4 u_View;
    vec3 u_ViewPosition;
} _122;

layout(binding = 3, std140) uniform Lights
{
    vec3 u_DirectionalLight;
    vec3 u_LightPos;
} _127;

layout(binding = 5, std140) uniform Specular
{
    vec3 u_SpecularColor;
    float u_Specular;
    float u_Shininess;
} _130;

layout(location = 0) out vec4 frag_color;
layout(location = 0) in vec3 v_Position;

void main()
{
    vec3 dawnColor = vec3(0.9700000286102294921875, 0.5, 0.070000000298023223876953125);
    vec3 skyColor = vec3(0.439999997615814208984375, 0.7799999713897705078125, 1.0);
    vec3 duskColor = vec3(0.7200000286102294921875, 0.4099999964237213134765625, 0.560000002384185791015625);
    vec3 nightColor = vec3(0.0);
    float weight0 = max(0.0, (-pow((_31.u_TimeOfDay + 1.0) * 4.0, 2.0)) + 1.0);
    float weight1 = max(0.0, (-pow((_31.u_TimeOfDay + 0.5) * 2.0, 8.0)) + 1.0);
    float weight2 = max(0.0, (-pow(_31.u_TimeOfDay * 4.0, 2.0)) + 1.0);
    float weight3 = max(0.0, (-pow((_31.u_TimeOfDay - 0.5) * 2.0, 8.0)) + 1.0);
    float weight4 = max(0.0, (-pow((_31.u_TimeOfDay - 1.0) * 4.0, 2.0)) + 1.0);
    vec3 baseColor = nightColor;
    baseColor += mix(nightColor, dawnColor, vec3(weight0));
    baseColor += mix(nightColor, skyColor, vec3(weight1));
    baseColor += mix(nightColor, duskColor, vec3(weight2));
    baseColor += mix(nightColor, dawnColor, vec3(weight4));
    frag_color = vec4(baseColor, 1.0);
}

    