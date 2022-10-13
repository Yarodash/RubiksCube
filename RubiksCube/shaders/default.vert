#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texcoord;
layout (location = 2) in float a_color;

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_proj;

uniform float piece_size;

out vec2 texcoord;
out float color;

void main() 
{
	gl_Position = m_proj * m_view * m_model * vec4(a_pos * piece_size, 1.0);
	texcoord = a_texcoord;
	color = a_color;
}
