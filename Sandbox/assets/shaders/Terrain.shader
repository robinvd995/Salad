                                  #version 450

layout(binding = 0, std140) uniform u_Scene
{
    mat4 u_Projection;
    mat4 u_View;
} _69;

layout(binding = 2, std140) uniform Transform
{
    mat4 u_Transform;
} _79;

layout(location = 0) in vec3 a_Position;
layout(location = 0) flat out vec3 v_Color;
layout(location = 1) out vec2 v_TexCoord;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) flat out vec3 v_Normal;
layout(location = 2) in vec3 a_Normal;

void main()
{
    vec3 pos = a_Position;
    if (a_Position.y < 0.5)
    {
        v_Color = vec3(0.800000011920928955078125, 0.60000002384185791015625, 0.20000000298023223876953125);
    }
    else
    {
        if (a_Position.y < 0.75)
        {
            v_Color = vec3(0.800000011920928955078125, 0.60000002384185791015625, 0.20000000298023223876953125);
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
    }
    gl_Position = ((_69.u_Projection * _69.u_View) * _79.u_Transform) * vec4(pos, 1.0);
    v_TexCoord = a_TexCoord;
    v_Normal = a_Normal;
}

    F  #version 450

layout(binding = 3, std140) uniform Lights
{
    vec3 u_DirectionalLight;
} _15;

layout(location = 2) flat in vec3 v_Normal;
layout(location = 0) flat in vec3 v_Color;
layout(location = 0) out vec4 frag_color;
layout(location = 1) in vec2 v_TexCoord;

void main()
{
    vec3 ambient = vec3(0.100000001490116119384765625);
    vec3 lightDirection = normalize(_15.u_DirectionalLight);
    vec3 norm = normalize(v_Normal);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = (vec3(diff) + ambient) * v_Color;
    frag_color = vec4(diffuse, 1.0);
}

    