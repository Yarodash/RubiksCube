#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texcoord;

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_proj;

out vec2 texcoord;

void main() {
	gl_Position = m_proj * m_view * m_model * vec4(a_pos, 1.0);
	texcoord = a_texcoord;
}
