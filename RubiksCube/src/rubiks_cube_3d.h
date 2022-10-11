#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
			}
		}

	public:
		Cube()
		{
			for (int i = 0; i < 27; i++)
			{
				float* data = create_colorless_piece();

				printf("%3d %3d %3d %3d\n", i, i % 3, (i / 3) % 3, (i / 9) % 3);
				glm::mat4 m_model = glm::mat4(1.0f);
				m_model = glm::translate(m_model, 
					glm::vec3(
						(i % 3 - 1) * 3.0f, 
						(i / 9 % 3 - 1) * -3.0f,
						(i / 3 % 3 - 1) * -3.0f
					)
				);

				for (int j = 0; j < 3 * 2 * 6; j++) {
					data[5 * j + 3] = i / 26.0;
					data[5 * j + 4] = i / 26.0;
				}

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
	};
}
