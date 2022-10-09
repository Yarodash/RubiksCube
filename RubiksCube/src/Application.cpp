#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "utils.h"


unsigned int compile_shader(unsigned int shader_type, const std::string& source)
{
    unsigned int shader_id = glCreateShader(shader_type);
    const char* source_c = source.c_str();
    glShaderSource(shader_id, 1, &source_c, nullptr);
    glCompileShader(shader_id);

    int result;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(shader_id, length, &length, message);

        printf("Failed to compile %s shader:\n%s",
            (shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment"),
            message);

        glDeleteShader(shader_id);
        exit(-1);
    }

    return shader_id;
}


unsigned int create_program(const std::string& vertex_shader, const std::string& fragment_shader)
{
    unsigned int program_id = glCreateProgram();

    unsigned int vertex_shader_id = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fragment_shader_id = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    glLinkProgram(program_id);
    glValidateProgram(program_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return program_id;
}


unsigned int create_program_from_files(const std::string& shaders_name)
{
    return create_program(
        get_file_contents("shaders/" + shaders_name + ".vert"),
        get_file_contents("shaders/" + shaders_name + ".frag")
    );
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
    ImGui::StyleColorsLight();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    float pos[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.0f,  0.5f,
    };

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), pos, GL_STATIC_DRAW);

    //unsigned int vao;
    //glGenVertexArrays(1, &vao);
    //glBindVertexArray(vao);

    //glEnableVertexArrayAttrib(vao, 0);
    //glVertexAttribPointer(0, 6 * sizeof(float), GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*) 0);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);

    unsigned int program = create_program_from_files("default");
    glUseProgram(program);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Another Window", NULL, ImGuiWindowFlags_MenuBar);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Menu"))
                {
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Examples"))
                {
                    ImGui::MenuItem("Main menu bar", NULL);
                    ImGui::MenuItem("Console", NULL);
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Tools"))
                {
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            ImGui::Text("Hello from another window!");
            ImGui::Button("Close Me");
            ImGui::End();
        }

        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        //glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
