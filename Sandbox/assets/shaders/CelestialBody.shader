                                @  #version 450

layout(binding = 0, std140) uniform u_Scene
{
    mat4 u_Projection;
    mat4 u_View;
    vec3 u_ViewPosition;
} _19;

layout(binding = 2, std140) uniform Transform
{
    mat4 u_Transform;
} _46;

layout(location = 0) out vec3 v_Position;
layout(location = 0) in vec3 a_Position;

void main()
{
    v_Position = a_Position;
    mat4 noTranslateView = _19.u_View;
    noTranslateView[3].x = 0.0;
    noTranslateView[3].y = 0.0;
    noTranslateView[3].z = 0.0;
    gl_Position = ((_19.u_Projection * noTranslateView) * _46.u_Transform) * vec4(a_Position, 1.0);
}

    �  #version 450

layout(binding = 0, std140) uniform u_Scene
{
    mat4 u_Projection;
    mat4 u_View;
    vec3 u_ViewPosition;
} _37;

layout(binding = 0) uniform sampler2D u_Texture;

layout(location = 0) in vec3 v_Position;
layout(location = 0) out vec4 frag_color;

void main()
{
    vec2 textureCoords = (v_Position.xy + vec2(1.0)) * vec2(0.5);
    vec4 texColor = texture(u_Texture, textureCoords);
    frag_color = texColor;
}

    