#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define _USE_MATH_DEFINES
#include "math.h"
#include <algorithm>


class Camera {
public:
	double u, v;
	double fov, aspect_ratio;
	double radius;

public:
	Camera(): u(0.5), v(0.5), radius(16.0), fov(45.0), aspect_ratio(1.0) {}

	void rotate(float delta_u, float delta_v) 
	{
		u += delta_u;
		v += delta_v;

		v = std::max(-M_PI * 0.48, std::min(M_PI * 0.48, v));
	}

	glm::mat4 get_view_matrix() 
	{
		float x = cos(v) * sin(u);
		float y = sin(v);
		float z = cos(v) * cos(u);

		glm::mat4 m_view = glm::lookAt(
			glm::vec3(x, y, z) * (float)radius,
			glm::vec3(0.0f), 
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		return m_view;
	}

	glm::mat4 get_proj_matrix()
	{
		glm::mat4 m_proj = glm::perspective(fov, aspect_ratio, 0.1, 40.0);

		return m_proj;
	}
};
