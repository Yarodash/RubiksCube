#version 330 core

in vec2 texcoord;

out vec4 f_color;

void main() {
	f_color = vec4(texcoord, 0.0, 1.0);
}
