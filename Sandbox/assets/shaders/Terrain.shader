                                U  #version 450

layout(binding = 0, std140) uniform u_Scene
{
    mat4 u_Projection;
    mat4 u_View;
} _64;

layout(binding = 2, std140) uniform Transform
{
    mat4 u_Transform;
} _74;

layout(location = 0) in vec3 a_Position;
layout(location = 0) flat out vec3 v_Color;
layout(location = 1) out vec2 v_TexCoord;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) out vec3 v_Normal;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) out vec3 v_FragPosition;

void main()
{
    vec3 pos = a_Position;
    if (a_Position.y < 2.2000000476837158203125)
    {
        v_Color = vec3(0.800000011920928955078125, 0.75, 0.4000000059604644775390625);
    }
    else
    {
        if (a_Position.y < 2.5)
        {
            v_Color = vec3(0.300000011920928955078125, 0.699999988079071044921875, 0.20000000298023223876953125);
        }
        else
        {
            if (a_Position.y < 5.0)
            {
                v_Color = vec3(0.20000000298023223876953125, 0.5, 0.100000001490116119384765625);
            }
            else
            {
                v_Color = vec3(0.89999997615814208984375, 0.89999997615814208984375, 1.0);
            }
        }
    }
    gl_Position = ((_64.u_Projection * _64.u_View) * _74.u_Transform) * vec4(pos, 1.0);
    v_TexCoord = a_TexCoord;
    v_Normal = a_Normal;
    v_FragPosition = a_Position;
}

    		  #version 450

layout(binding = 3, std140) uniform Lights
{
    vec3 u_DirectionalLight;
} _176;

layout(binding = 2) uniform sampler2D u_RockTexture;
layout(binding = 0) uniform sampler2D u_SandTexture;
layout(binding = 1) uniform sampler2D u_GrassTexture;
layout(binding = 3) uniform sampler2D u_SnowTexture;

layout(location = 3) in vec3 v_FragPosition;
layout(location = 2) in vec3 v_Normal;
layout(location = 0) out vec4 frag_color;
layout(location = 0) flat in vec3 v_Color;
layout(location = 1) in vec2 v_TexCoord;

void main()
{
    float gradientStart = 2.0;
    float gradientEnd = 3.2000000476837158203125;
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
    vec3 sampleX = mix(mix(sampleX0, sampleX1, vec3(heightBlendFactor)), sampleX2, vec3(snowBlendFactor));
    vec3 sampleY = mix(mix(sampleY0, sampleY1, vec3(heightBlendFactor)), sampleY2, vec3(snowBlendFactor));
    vec3 sampleZ = mix(mix(sampleZ0, sampleZ1, vec3(heightBlendFactor)), sampleZ2, vec3(snowBlendFactor));
    vec3 weights = abs(v_Normal);
    weights /= vec3((weights.x + weights.y) + weights.z);
    vec3 tex = ((sampleX * weights.x) + (sampleY * weights.y)) + (sampleZ * weights.z);
    vec3 ambient = vec3(0.100000001490116119384765625);
    vec3 lightDirection = normalize(_176.u_DirectionalLight);
    vec3 norm = normalize(v_Normal);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = (vec3(diff) + ambient) * tex;
    frag_color = vec4(diffuse, 1.0);
}

    