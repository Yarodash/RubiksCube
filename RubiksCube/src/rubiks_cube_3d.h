#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <deque>
#include "camera.h"

namespace Cube3D 
{
	struct Piece
	{
		unsigned int vbo;
		unsigned int vao;
		glm::mat4 m_model;

		Piece() : vbo(-1), vao(-1), m_model(glm::mat4()) {}
		Piece(unsigned int _vbo, unsigned int _vao, glm::mat4 _m_model) : vbo(_vbo), vao(_vao), m_model(_m_model) {}
	};

	class Cube
	{
		Piece pieces[27];

		float* create_colorless_piece() {
			float* data = new float[5 * 3 * 2 * 6]
			{
				+1.0f, -1.0f, +1.0f, 0.0f, 0.0f,
				+1.0f, +1.0f, +1.0f, 0.0f, 0.0f,
				+1.0f, +1.0f, -1.0f, 0.0f, 0.0f,
				+1.0f, -1.0f, +1.0f, 0.0f, 0.0f,
				+1.0f, +1.0f, -1.0f, 0.0f, 0.0f,
				+1.0f, -1.0f, -1.0f, 0.0f, 0.0f,

				-1.0f, -1.0f, +1.0f, 0.0f, 0.0f,
				-1.0f, +1.0f, +1.0f, 0.0f, 0.0f,
				-1.0f, +1.0f, -1.0f, 0.0f, 0.0f,
				-1.0f, -1.0f, +1.0f, 0.0f, 0.0f,
				-1.0f, +1.0f, -1.0f, 0.0f, 0.0f,
				-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,

				-1.0f, +1.0f, +1.0f, 0.0f, 0.0f,
				+1.0f, +1.0f, +1.0f, 0.0f, 0.0f,
				+1.0f, +1.0f, -1.0f, 0.0f, 0.0f,
				-1.0f, +1.0f, +1.0f, 0.0f, 0.0f,
				+1.0f, +1.0f, -1.0f, 0.0f, 0.0f,
				-1.0f, +1.0f, -1.0f, 0.0f, 0.0f,

				-1.0f, -1.0f, +1.0f, 0.0f, 0.0f,
				+1.0f, -1.0f, +1.0f, 0.0f, 0.0f,
				+1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
				-1.0f, -1.0f, +1.0f, 0.0f, 0.0f,
				+1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
				-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,

				-1.0f, +1.0f, +1.0f, 0.0f, 0.0f,
				+1.0f, +1.0f, +1.0f, 0.0f, 0.0f,
				+1.0f, -1.0f, +1.0f, 0.0f, 0.0f,
				-1.0f, +1.0f, +1.0f, 0.0f, 0.0f,
				+1.0f, -1.0f, +1.0f, 0.0f, 0.0f,
				-1.0f, -1.0f, +1.0f, 0.0f, 0.0f,

				-1.0f, +1.0f, -1.0f, 0.0f, 0.0f,
				+1.0f, +1.0f, -1.0f, 0.0f, 0.0f,
				+1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
				-1.0f, +1.0f, -1.0f, 0.0f, 0.0f,
				+1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
				-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
			};

			return data;
		}

		void set_color(float* data, int direction, int color) {

			float tx[6] = { 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f };
			float ty[6] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f };

			for (int i = 0; i < 6; i++)
				tx[i] = (tx[i] + color) / 7.0f;

			for (int i = 0; i < 6; i++)
			{
				data[direction * 30 + i * 5 + 3] = tx[i];
				data[direction * 30 + i * 5 + 4] = ty[i];

				data[direction * 30 + i * 5 + 3] = color / 6.0f;
				data[direction * 30 + i * 5 + 4] = color / 6.0f;
			}
		}

	public:
		Cube(Cube* other)
		{
			for (int i = 0; i < 27; i++)
				pieces[i] = other->pieces[i];
		}

		Cube()
		{
			int colors[162] = {
				0, 2, 3, 0, 5, 0,
				0, 0, 3, 0, 5, 0,
				1, 0, 3, 0, 5, 0,
				0, 2, 3, 0, 0, 0,
				0, 0, 3, 0, 0, 0,
				1, 0, 3, 0, 0, 0,
				0, 2, 3, 0, 0, 6,
				0, 0, 3, 0, 0, 6,
				1, 0, 3, 0, 0, 6,
				0, 2, 0, 0, 5, 0,
				0, 0, 0, 0, 5, 0,
				1, 0, 0, 0, 5, 0,
				0, 2, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0,
				1, 0, 0, 0, 0, 0,
				0, 2, 0, 0, 0, 6,
				0, 0, 0, 0, 0, 6,
				1, 0, 0, 0, 0, 6,
				0, 2, 0, 4, 5, 0,
				0, 0, 0, 4, 5, 0,
				1, 0, 0, 4, 5, 0,
				0, 2, 0, 4, 0, 0,
				0, 0, 0, 4, 0, 0,
				1, 0, 0, 4, 0, 0,
				0, 2, 0, 4, 0, 6,
				0, 0, 0, 4, 0, 6,
				1, 0, 0, 4, 0, 6,
			};

			float* data = create_colorless_piece();

			for (int i = 0; i < 27; i++)
			{
				glm::mat4 m_model = glm::mat4(1.0f);

				m_model = glm::translate(m_model, 
					glm::vec3(
						(i % 3 - 1) * 2.2f, 
						(i / 9 % 3 - 1) * -2.2f,
						(i / 3 % 3 - 1) * -2.2f
					)
				);	

				for (int j = 0; j < 6; j++)
					set_color(data, j, colors[i * 6 + j]);

				unsigned int vbo;
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, 5 * 3 * 2 * 6 * sizeof(float), data, GL_STATIC_DRAW);

				unsigned int vao;
				glGenVertexArrays(1, &vao);
				glBindVertexArray(vao);

				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0);

				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(3 * sizeof(float)));

				pieces[i] = Piece(vbo, vao, m_model);
			}

			delete[] data;
		}

		void render(unsigned int program)
		{
			glUseProgram(program);

			int m_model_uniform = glGetUniformLocation(program, "m_model");

			for (int i = 0; i < 27; i++)
			{
				glUniformMatrix4fv(m_model_uniform, 1, GL_FALSE, &(pieces[i].m_model[0][0]));
				glBindVertexArray(pieces[i].vao);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		void rotate(int axis, float rotation_percent)
		{
			Piece new_pieces[27];

			for (int i = 0; i < 27; i++)
				new_pieces[i] = pieces[i];

			int rotations[] = {
				2, 5, 8, 17, 26, 23, 20, 11,
				0, 9, 18, 21, 24, 15, 6, 3,
				0, 3, 6, 7, 8, 5, 2, 1,
				18, 19, 20, 23, 26, 25, 24, 21,
				0, 1, 2, 11, 20, 19, 18, 9,
				8, 7, 6, 15, 24, 25, 26, 17,
			};
			glm::vec3 matrix_rotation[] = {
				glm::vec3(1, 0, 0),
				glm::vec3(-1, 0, 0),
				glm::vec3(0, 1, 0),
				glm::vec3(0, -1, 0),
				glm::vec3(0, 0, 1),
				glm::vec3(0, 0, -1)
			};
			int centers[] = { 14, 12, 4, 22, 10, 16 };

			for (int i = 0; i < 8; i++) 
			{
				new_pieces[rotations[axis * 8 + i]] = pieces[rotations[axis * 8 + (i + 6) % 8]];
				new_pieces[rotations[axis * 8 + i]].m_model =
					glm::rotate(
						glm::mat4(1.0),
						(float)M_PI / -2.0f * rotation_percent,
						glm::vec3(matrix_rotation[axis])
					) * new_pieces[rotations[axis * 8 + i]].m_model;
			}

			new_pieces[centers[axis]].m_model =
				glm::rotate(
					glm::mat4(1.0),
					(float)M_PI / -2.0f * rotation_percent,
					glm::vec3(matrix_rotation[axis])
				) * new_pieces[centers[axis]].m_model;

			for (int i = 0; i < 27; i++)
				pieces[i] = new_pieces[i];
		}

		Cube copy() 
		{
			return Cube(this);
		}
	};

	class CubeManager {
	private:
		Camera camera;

		Cube original_cube;
		Cube current_cube;
		Cube rotated_cube;

		float rotation_percent = 0.0f;
		float percent_per_second = 1.0f;

		std::deque<int> rotation_queue;

	public:
		CubeManager() 
		{
			original_cube = Cube();
			current_cube = original_cube.copy();
			rotated_cube = current_cube.copy();

			camera = Camera();

			rotation_queue = std::deque<int>();
		}

		void add_rotation(int axis) 
		{
			rotation_queue.push_back(axis);
		}

		void update(float delta_time)
		{
			if (rotation_queue.size() == 0)
				return;
			
			int axis = rotation_queue.front();

			rotation_percent += percent_per_second * delta_time;
			if (rotation_percent >= 1.0f) {
				delta_time = (rotation_percent - 1.0f) / percent_per_second;
				rotation_percent = 1.0f;
			}
			else {
				delta_time = 0.0f;
			}

			if (rotation_percent < 1.0f) {
				rotated_cube = current_cube.copy();
				rotated_cube.rotate(axis, rotation_percent);
			}
			else {
				current_cube.rotate(axis, 1.0f);
				rotated_cube = current_cube.copy();
				rotation_queue.pop_front();
				rotation_percent = 0.0f;
			}

			if (delta_time > 0.0f)
				update(delta_time);
		}

		void render(unsigned int program) 
		{
			glm::mat4 m_view = camera.get_view_matrix();
			glm::mat4 m_proj = camera.get_proj_matrix();

			glUniformMatrix4fv(glGetUniformLocation(program, "m_view"), 1, GL_FALSE, &m_view[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(program, "m_proj"), 1, GL_FALSE, &m_proj[0][0]);

			rotated_cube.render(program);
		}

		void rotate_camera(float mouse_x, float mouse_y)
		{
			camera.rotate(-mouse_x, mouse_y);
		}
	};
}
