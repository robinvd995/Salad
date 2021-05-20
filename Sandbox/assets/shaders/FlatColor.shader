                                x  #version 450

layout(binding = 0, std140) uniform u_Scene
{
    mat4 u_Projection;
    mat4 u_View;
} _19;

layout(binding = 2, std140) uniform Transform
{
    mat4 u_Transform;
} _29;

layout(binding = 1, std140) uniform u_Material
{
    vec4 u_Color;
} _48;

layout(location = 0) in vec3 a_Position;
layout(location = 0) out vec4 v_Color;
layout(location = 1) out vec2 v_TexCoord;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

void main()
{
    gl_Position = ((_19.u_Projection * _19.u_View) * _29.u_Transform) * vec4(a_Position, 1.0);
    v_Color = _48.u_Color;
    v_TexCoord = a_TexCoord;
}

      #version 450

layout(binding = 0) uniform sampler2D u_DiffuseMap;

layout(location = 0) out vec4 frag_color;
layout(location = 1) in vec2 v_TexCoord;
layout(location = 0) in vec4 v_Color;

void main()
{
    frag_color = texture(u_DiffuseMap, v_TexCoord) * v_Color;
}

    