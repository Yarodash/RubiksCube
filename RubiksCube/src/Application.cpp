#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "math.h"
#include <iostream>
#include "utils.h"
#include <random>

#include "Scrambles.h"
#include "rubiks_cube_3d.h"

int main(void)
{
    srand(time(NULL));

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    /* Create a windowed mode window and its OpenGL context */

    int display_w = 720, display_h = 720;

    window = glfwCreateWindow(display_w, display_h, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return -1;
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    /*ImGui::CreateContext();
    //ImGui::StyleColorsLight();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");*/

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    unsigned int program = create_program_from_files("default");

    glm::mat4 m_proj = glm::perspective(45.0f, 1.0f, 0.1f, 40.0f);
    glm::mat4 m_view = glm::lookAt(glm::vec3(3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 m_model = glm::mat4();

    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "m_view"), 1, GL_FALSE, &m_view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(program, "m_proj"), 1, GL_FALSE, &m_proj[0][0]);
    //glUniformMatrix4fv(glGetUniformLocation(program, "m_model"), 1, GL_FALSE, &m_model[0][0]);
    {
        Cube3D::Cube cube = Cube3D::Cube();

        float i = 0;

        glEnable(GL_DEPTH_TEST);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /*ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            {
                ImGui::SetNextWindowPos(ImVec2(0, 0));
                ImGui::SetNextWindowSize(ImVec2(display_w, display_h));

                ImGui::Begin("Another Window", NULL, ImGuiWindowFlags_MenuBar
                    | ImGuiWindowFlags_NoTitleBar
                    | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoResize);

                ImGui::Button("Close Me");
                ImGui::End();
            }

            ImGui::Render();
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glClearColor(0, 0, 0, 0);
            glEnable(GL_SCISSOR_TEST);
            glScissor(100, 100, 200, 200);
            glClear(GL_COLOR_BUFFER_BIT);
            glDisable(GL_SCISSOR_TEST); */

            glClearColor(0.08, 0.16, 0.18, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            i++;
            glm::mat4 m_view = glm::lookAt(glm::vec3(15.0f * sin(i / 100), 15.0f * cos(i / 40), 15.0f * cos(i / 100)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(glGetUniformLocation(program, "m_view"), 1, GL_FALSE, &m_view[0][0]);

            cube.render(program);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        /*ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();*/

    }

    glfwTerminate();
    return 0;
}
