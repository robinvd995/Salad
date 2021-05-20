                                �  #version 450

layout(binding = 1, std140) uniform u_Material
{
    vec4 u_BumpProperties0;
    vec4 u_BumpProperties1;
} _33;

layout(binding = 4, std140) uniform u_Time
{
    float u_Timer;
    float u_NormalizedTimer;
} _43;

layout(binding = 0, std140) uniform u_Scene
{
    mat4 u_Projection;
    mat4 u_View;
} _112;

layout(binding = 2, std140) uniform Transform
{
    mat4 u_Transform;
} _121;

layout(binding = 0) uniform sampler2D u_WaterBump0;
layout(binding = 1) uniform sampler2D u_WaterBump1;

layout(location = 1) out vec2 v_TextureCoords;
layout(location = 0) in vec3 a_Position;
layout(location = 0) out float v_SurfaceDistance;
layout(location = 2) in float a_SurfaceDistance;
layout(location = 1) in vec3 a_Normal;

void main()
{
    v_TextureCoords = vec2(a_Position.x, a_Position.z);
    float bumpSample0 = (textureLod(u_WaterBump0, (v_TextureCoords / vec2(_33.u_BumpProperties0.x)) + vec2(_43.u_Timer * _33.u_BumpProperties0.y), 0.0).x * _33.u_BumpProperties0.z) - _33.u_BumpProperties0.w;
    float bumpSample1 = (textureLod(u_WaterBump1, ((-v_TextureCoords) / vec2(_33.u_BumpProperties1.x)) + vec2(_43.u_Timer * _33.u_BumpProperties1.y), 0.0).x * _33.u_BumpProperties1.z) - _33.u_BumpProperties1.w;
    float height = bumpSample0 + bumpSample1;
    vec3 pos = a_Position;
    pos.y += height;
    v_SurfaceDistance = a_SurfaceDistance + height;
    gl_Position = ((_112.u_Projection * _112.u_View) * _121.u_Transform) * vec4(pos, 1.0);
}

    z  #version 450

layout(binding = 1, std140) uniform u_Material
{
    vec4 u_BumpProperties0;
    vec4 u_BumpProperties1;
} _54;

layout(location = 0) in float v_SurfaceDistance;
layout(location = 0) out vec4 frag_color;
layout(location = 1) in vec2 v_TextureCoords;

void main()
{
    if (v_SurfaceDistance < 0.0)
    {
        discard;
    }
    float maxSurfaceDistance = 0.800000011920928955078125;
    float surfaceDistance = min(v_SurfaceDistance, maxSurfaceDistance) / maxSurfaceDistance;
    vec3 waterColor = mix(vec3(0.20000000298023223876953125, 0.699999988079071044921875, 0.89999997615814208984375), vec3(0.20000000298023223876953125, 0.300000011920928955078125, 0.89999997615814208984375), vec3(surfaceDistance));
    if (surfaceDistance < 0.07999999821186065673828125)
    {
        waterColor = vec3(0.89999997615814208984375);
    }
    frag_color = vec4(waterColor, 1.0);
}

    