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

#include "scrambles.h"
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

    glfwWindowHint(GLFW_SAMPLES, 4);

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

    ImGui::CreateContext();
    //ImGui::StyleColorsLight();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    unsigned int program = create_program_from_files("default");

    glEnable(GL_MULTISAMPLE);

    glUseProgram(program);
    //glUniformMatrix4fv(glGetUniformLocation(program, "m_model"), 1, GL_FALSE, &m_model[0][0]);
    {
        Cube3D::CubeManager cube = Cube3D::CubeManager();

        glEnable(GL_DEPTH_TEST);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Poll for and process events */
            glfwPollEvents();

            glClearColor(0.08, 0.16, 0.18, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            {
                ImGui::SetNextWindowPos(ImVec2(0, 0));
                ImGui::SetNextWindowSize(ImVec2(display_w, display_h));

                ImGui::Begin("Another Window", NULL, ImGuiWindowFlags_MenuBar
                    | ImGuiWindowFlags_NoTitleBar
                    | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoResize);

                if (ImGui::Button("R turn"))
                    cube.add_rotation(0);

                if (ImGui::Button("L turn"))
                    cube.add_rotation(1);

                if (ImGui::Button("U turn"))
                    cube.add_rotation(2);

                if (ImGui::Button("D turn"))
                    cube.add_rotation(3);

                if (ImGui::Button("F turn"))
                    cube.add_rotation(4);

                if (ImGui::Button("B turn"))
                    cube.add_rotation(5);

                if (ImGui::Button("Reset"))
                    cube.reset();

                ImGui::End();
            }

            ImGui::Render();
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /*glClearColor(0, 0, 0, 0);
            glEnable(GL_SCISSOR_TEST);
            glScissor(100, 100, 200, 200);
            glClear(GL_COLOR_BUFFER_BIT);
            glDisable(GL_SCISSOR_TEST);*/

            ImGuiIO& io = ImGui::GetIO();

            cube.camera.aspect_ratio = (float)display_w / display_h;
            if (io.MouseDown[0])
                cube.rotate_camera(io.MouseDelta.x / 300.0, io.MouseDelta.y / 300.0);

            cube.update(io.DeltaTime);
            cube.render(program);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    glfwTerminate();
    return 0;
}
