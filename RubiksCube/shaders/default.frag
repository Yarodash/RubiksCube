#version 330 core

in vec2 texcoord;
in float color;

out vec4 f_color;

float max2(vec2 v) {
  return max(v.x, v.y);
}

vec3 get_color() {
	float color = round(color);

	if (color == 0.0)
		return vec3(0.0);

	if (color == 1.0)
		return vec3(1.0, 0.0, 0.0);

	if (color == 2.0)
		return vec3(1.0, 0.3, 0.0);

	if (color == 3.0)
		return vec3(1.0, 1.0, 1.0);

	if (color == 4.0)
		return vec3(1.0, 1.0, 0.0);

	if (color == 5.0)
		return vec3(0.0, 1.0, 0.0);

	if (color == 6.0)
		return vec3(0.0, 0.0, 1.0);
}

vec3 get_shading(vec3 color) {
	float radius = max(1 - distance(texcoord, vec2(0.3)) / sqrt(2), 0.0) * 0.9 + 0.1;
	float koef = pow((0.5 * pow(radius, 1) + 0.5), 3);
	return pow(color * koef, vec3(0.5));
}

vec3 get_borders(vec3 color) {
	float l = pow(max2(abs(texcoord - vec2(0.5))), 3.16) * 4.778;
	l = pow(smoothstep(0.5, 0.365, l), 4.0);
	return color * l;
}

void main() 
{
	vec3 result_color = get_color();

	result_color = get_shading(result_color);

	result_color = get_borders(result_color);

	f_color = vec4(pow(result_color, vec3(1.2)), 1.0);
}
