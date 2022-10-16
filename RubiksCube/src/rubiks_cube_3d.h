#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <deque>
#include "camera.h"

#include "scrambles.h"

namespace Cube3D 
{
	struct Piece
	{
		unsigned int vbo;
		unsigned int vao;
		glm::mat4 m_model;

		Piece() : vbo(-1), vao(-1), m_model(glm::mat4()) {}
		Piece(unsigned int _vbo, unsigned int _vao, glm::mat4 _m_model) : vbo(_vbo), vao(_vao), m_model(_m_model) {}

		void destroy()
		{
			glDeleteBuffers(1, &vbo);
			glDeleteVertexArrays(1, &vao);
		}
	};

	class Cube
	{
		Piece pieces[27];

		float* create_colorless_piece() {
			float* data = new float[6 * 3 * 2 * 6]
			{
				+1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 0.0f,
				+1.0f, +1.0f, +1.0f, 0.0f, 0.0f, 0.0f,
				+1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				+1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 0.0f,
				+1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				+1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,

				-1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 0.0f,
				-1.0f, +1.0f, +1.0f, 0.0f, 0.0f, 0.0f,
				-1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				-1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 0.0f,
				-1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,

				-1.0f, +1.0f, +1.0f, 0.0f, 0.0f, 0.0f,
				+1.0f, +1.0f, +1.0f, 0.0f, 0.0f, 0.0f,
				+1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				-1.0f, +1.0f, +1.0f, 0.0f, 0.0f, 0.0f,
				+1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				-1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 0.0f,

				-1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 0.0f,
				+1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 0.0f,
				+1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				-1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 0.0f,
				+1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,

				-1.0f, +1.0f, +1.0f, 0.0f, 0.0f, 0.0f,
				+1.0f, +1.0f, +1.0f, 0.0f, 0.0f, 0.0f,
				+1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 0.0f,
				-1.0f, +1.0f, +1.0f, 0.0f, 0.0f, 0.0f,
				+1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 0.0f,
				-1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 0.0f,

				-1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				+1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				+1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				-1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				+1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			};

			return data;
		}

		void set_color(float* data, int direction, int color) {

			float tx[6] = { 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f };
			float ty[6] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f };

			for (int i = 0; i < 6; i++)
			{
				data[direction * 36 + i * 6 + 3] = tx[i];
				data[direction * 36 + i * 6 + 4] = ty[i];
				data[direction * 36 + i * 6 + 5] = color;
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
						(i % 3 - 1) * 2.15f, 
						(i / 9 % 3 - 1) * -2.15f,
						(i / 3 % 3 - 1) * -2.15f
					)
				);	

				for (int j = 0; j < 6; j++)
					set_color(data, j, colors[i * 6 + j]);

				unsigned int vbo;
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, 6 * 3 * 2 * 6 * sizeof(float), data, GL_STATIC_DRAW);

				unsigned int vao;
				glGenVertexArrays(1, &vao);
				glBindVertexArray(vao);

				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)0);

				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(3 * sizeof(float)));

				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(5 * sizeof(float)));

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

		void rotate(int axis, float rotation_percent, bool inverse=false)
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

			float rotation_sign = (inverse ? 1.0f : -1.0f);
			int array_shift = (inverse ? 2 : 6);

			for (int i = 0; i < 8; i++) 
			{
				new_pieces[rotations[axis * 8 + i]] = pieces[rotations[axis * 8 + (i + array_shift) % 8]];
				new_pieces[rotations[axis * 8 + i]].m_model =
					glm::rotate(
						glm::mat4(1.0),
						(float)M_PI / 2.0f * rotation_percent * rotation_sign,
						glm::vec3(matrix_rotation[axis])
					) * new_pieces[rotations[axis * 8 + i]].m_model;
			}

			new_pieces[centers[axis]].m_model =
				glm::rotate(
					glm::mat4(1.0),
					(float)M_PI / 2.0f * rotation_percent * rotation_sign,
					glm::vec3(matrix_rotation[axis])
				) * new_pieces[centers[axis]].m_model;

			for (int i = 0; i < 27; i++)
				pieces[i] = new_pieces[i];
		}

		void destroy()
		{
			for (int i = 0; i < 27; i++)
				pieces[i].destroy();
		}

		Cube copy() 
		{
			return Cube(this);
		}
	};

	class CubeManager {
	public:
		Camera camera;

		Cube original_cube;
		Cube current_cube;
		Cube rotated_cube;

		float rotation_percent = 0.0f;
		float percent_per_second = 1.0f;

		float reset_percent = 1.0f;
		float reset_percent_per_second = 1.0f;
		bool in_reset_stage = false;

		std::deque<std::pair<int, bool>> rotation_queue;
		std::deque<std::pair<int, bool>> rotation_queue_buffer;

	public:
		CubeManager() 
		{
			original_cube = Cube();
			current_cube = original_cube.copy();
			rotated_cube = current_cube.copy();

			camera = Camera();

			rotation_queue = std::deque<std::pair<int, bool>>();
			rotation_queue_buffer = std::deque<std::pair<int, bool>>();
		}

		~CubeManager()
		{
			original_cube.destroy();
		}

		void add_rotation(int axis, bool inverse=false) 
		{
			if (in_reset_stage)
				rotation_queue_buffer.push_back(std::make_pair(axis, inverse));
			else
				rotation_queue.push_back(std::make_pair(axis, inverse));
		}

		void update(float delta_time)
		{
			if (in_reset_stage) {
				float new_reset_percent = reset_percent + reset_percent_per_second * delta_time;

				if (reset_percent < 0.5 && new_reset_percent >= 0.5) {
					rotated_cube = original_cube.copy();
					current_cube = rotated_cube.copy();
					rotation_queue.clear();
				}

				if (new_reset_percent >= 1.0f) {
					in_reset_stage = false;
					new_reset_percent = 1.0f;
					rotation_percent = 0.0f;

					while (rotation_queue_buffer.size() > 0) {
						rotation_queue.push_back(rotation_queue_buffer.front());
						rotation_queue_buffer.pop_front();
					}
				}

				reset_percent = new_reset_percent;
			}

			if (rotation_queue.size() == 0)
				return;

			std::pair<int, bool> rotation = rotation_queue.front();
			int axis = rotation.first;
			bool inverse = rotation.second;

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
				rotated_cube.rotate(axis, rotation_percent, inverse);
			}
			else {
				current_cube.rotate(axis, 1.0f, inverse);
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

			float piece_size = (reset_percent <= 0.5 ? (0.5 - reset_percent) * 2.0 : (reset_percent - 0.5) * 2.0);
			glUniform1f(glGetUniformLocation(program, "piece_size"), piece_size);

			rotated_cube.render(program);
		}

		void reset() 
		{
			if (reset_percent != 1.0) {
				if (reset_percent > 0.5)
					reset_percent = 1 - reset_percent;
				return;
			}

			in_reset_stage = true;
			reset_percent = 0.0f;
		}

		void reset_rotation_buffer() {
			rotation_queue_buffer.clear();
		}

		void apply_scramble(Scrambles::RotationList* scramble) 
		{
			scramble->reset_index();

			while (scramble->has_more()) 
			{
				Scrambles::Rotation rotation = scramble->next();

				if (rotation.count <= 2) 
					add_rotation(rotation._axis);

				if (rotation.count == 2)
					add_rotation(rotation._axis);

				if (rotation.count == 3)
					add_rotation(rotation._axis, true);
			}
		}

		void rotate_camera(float mouse_x, float mouse_y)
		{
			camera.rotate(-mouse_x, mouse_y);
		}
	};
}
