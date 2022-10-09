#version 330 core

in vec2 v_pos;

out vec4 f_color;

void main() {
	f_color = vec4(v_pos, 0.0, 1.0);
}
