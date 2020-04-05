// Basic Texture shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec4 a_Color;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoord;
out vec4 v_Color;

void main()
{
  v_Color = a_Color;
  v_TexCoord = a_TexCoord;
  gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform int u_TilingFactor;

void main()
{
  color = texture(u_Texture, v_TexCoord * u_TilingFactor) * v_Color;
}
