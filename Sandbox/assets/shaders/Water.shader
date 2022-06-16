                                  #version 450

layout(binding = 1, std140) uniform u_Material
{
    vec4 u_BumpProperties0;
    vec4 u_BumpProperties1;
} _28;

layout(binding = 4, std140) uniform u_Time
{
    float u_Timer;
    float u_NormalizedTimer;
} _38;

layout(binding = 2, std140) uniform Transform
{
    mat4 u_Transform;
} _136;

layout(binding = 0, std140) uniform u_Scene
{
    mat4 u_Projection;
    mat4 u_View;
    vec3 u_ViewPosition;
} _157;

layout(binding = 0) uniform sampler2D u_WaterBump0;
layout(binding = 1) uniform sampler2D u_WaterBump1;
layout(binding = 2) uniform sampler2D u_WaterNormal0;
layout(binding = 3) uniform sampler2D u_WaterNormal1;

layout(location = 1) out vec2 v_TextureCoords;
layout(location = 0) in vec3 a_Position;
layout(location = 2) out vec3 v_Normal;
layout(location = 0) out float v_SurfaceDistance;
layout(location = 2) in float a_SurfaceDistance;
layout(location = 3) out vec3 v_FragPosition;
layout(location = 1) in vec3 a_Normal;

void main()
{
    v_TextureCoords = vec2(a_Position.x, a_Position.z);
    vec2 sampleCoords0 = (v_TextureCoords / vec2(_28.u_BumpProperties0.x)) + vec2(_38.u_Timer * _28.u_BumpProperties0.y);
    vec2 sampleCoords1 = ((-v_TextureCoords) / vec2(_28.u_BumpProperties1.x)) + vec2(_38.u_Timer * _28.u_BumpProperties1.y);
    float bumpSample0 = (textureLod(u_WaterBump0, sampleCoords0, 0.0).x * _28.u_BumpProperties0.z) - _28.u_BumpProperties0.w;
    float bumpSample1 = (textureLod(u_WaterBump1, sampleCoords1, 0.0).x * _28.u_BumpProperties1.z) - _28.u_BumpProperties1.w;
    vec3 normSample0 = textureLod(u_WaterNormal0, sampleCoords0, 0.0).xzy - vec3(0.5, 0.0, 0.5);
    vec3 normSample1 = textureLod(u_WaterNormal1, sampleCoords1, 0.0).xzy - vec3(0.5, 0.0, 0.5);
    v_Normal = normalize(normSample0 + normSample1);
    float height = bumpSample0 + bumpSample1;
    vec3 pos = a_Position;
    pos.y += height;
    v_SurfaceDistance = a_SurfaceDistance + height;
    v_FragPosition = vec3((_136.u_Transform * vec4(pos, 1.0)).xyz);
    gl_Position = ((_157.u_Projection * _157.u_View) * _136.u_Transform) * vec4(pos, 1.0);
}

    ]  #version 450

layout(binding = 3, std140) uniform Lights
{
    vec3 u_DirectionalLight;
} _49;

layout(binding = 0, std140) uniform u_Scene
{
    mat4 u_Projection;
    mat4 u_View;
    vec3 u_ViewPosition;
} _76;

layout(binding = 5, std140) uniform Specular
{
    vec3 u_SpecularColor;
    float u_Specular;
    float u_Shininess;
} _96;

layout(binding = 1, std140) uniform u_Material
{
    vec4 u_BumpProperties0;
    vec4 u_BumpProperties1;
} _125;

layout(location = 0) in float v_SurfaceDistance;
layout(location = 2) in vec3 v_Normal;
layout(location = 3) in vec3 v_FragPosition;
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
    vec3 ambient = vec3(0.100000001490116119384765625);
    vec3 lightDirection = normalize(_49.u_DirectionalLight);
    vec3 norm = normalize(v_Normal);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = vec3(diff) + ambient;
    vec3 viewDir = normalize(_76.u_ViewPosition - v_FragPosition);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), _96.u_Shininess);
    vec3 specular = _96.u_SpecularColor * (spec * _96.u_Specular);
    vec3 result = (waterColor * diffuse) + specular;
    frag_color = vec4(result, 0.89999997615814208984375);
}

    