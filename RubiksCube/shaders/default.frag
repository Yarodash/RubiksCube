#version 330 core

in vec2 texcoord;

out vec4 f_color;

void main() {

	vec3 color;

	if (texcoord.x <= 1.0 / 7.0)
		color = vec3(0.01);

	else if (texcoord.x <= 2.0 / 7.0)
		color = vec3(1.0, 0.0, 0.0);

	else if (texcoord.x <= 3.0 / 7.0)
		color = vec3(1.0, 0.5, 0.0);

	else if (texcoord.x <= 4.0 / 7.0)
		color = vec3(1.0, 1.0, 1.0);

	else if (texcoord.x <= 5.0 / 7.0)
		color = vec3(1.0, 1.0, 0.0);

	else if (texcoord.x <= 6.0 / 7.0)
		color = vec3(0.0, 1.0, 0.0);

	else if (texcoord.x <= 7.0 / 7.0)
		color = vec3(0.0, 0.0, 1.0);


	f_color = vec4(color, 1.0);
}
