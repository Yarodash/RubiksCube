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
#include "results_table.h"

#include <memory>

GLFWwindow* window;
int display_w = 1200, display_h = 720;
unsigned int program;

struct AppData {
    Cube3D::CubeManager cube = Cube3D::CubeManager();
    Results results = Results();
    std::shared_ptr<Scrambles::RotationList> last_scramble_ptr;

    // 0 - timer stopped
    // 1 - show scramble
    // 2 - timer running
    int stage = 0;

    float timer_time = 0.0f;
    bool ignore_space = false;
    bool ready_to_start = false;
    bool cursor_rotate_cube = false;

    float rotation_per_sec = 1.2f;

    std::string scramble_window_text = "Press SPACE to generate scramble";
    ImVec4 scramble_color = ImVec4(1.0, 1.0, 1.0, 1.0);
    ImVec4 timer_color = ImVec4(1.0, 1.0, 1.0, 1.0);

    ImGuiIO& io = ImGui::GetIO();

    ImFont* default_font = io.Fonts->AddFontFromFileTTF("fonts\\GoogleSans-Regular.ttf", 20.0f);
    ImFont* table_font_big = io.Fonts->AddFontFromFileTTF("fonts\\Digital.ttf", 21.0f);
    ImFont* table_font_small = io.Fonts->AddFontFromFileTTF("fonts\\Digital.ttf", 16.0f);
    ImFont* scramble_font = io.Fonts->AddFontFromFileTTF("fonts\\Digital.ttf", 26.0f);
    ImFont* timer_font = io.Fonts->AddFontFromFileTTF("fonts\\Digital.ttf", 48.0f);

    AppData() {}
};

int init() 
{
    srand(time(NULL));

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    glfwWindowHint(GLFW_SAMPLES, 4);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(display_w, display_h, "Rubik's cube", NULL, NULL);
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    program = create_program_from_files("default");
    glUseProgram(program);

    return 0;
}

void deinit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

void set_stage_0(AppData& app_data, bool ignore_time=false)
{
    app_data.scramble_color = ImVec4(1.0, 1.0, 1.0, 1.0);
    app_data.timer_color = ImVec4(1.0, 1.0, 1.0, 1.0);

    app_data.scramble_window_text = "Press SPACE to generate scramble";
    
    app_data.cube.percent_per_second = app_data.rotation_per_sec;

    app_data.timer_color = (ignore_time ? ImVec4(1.0, 1.0, 1.0, 1.0) : ImVec4(0.5, 1.0, 0.5, 1.0));
    
    app_data.cube.reset();
    app_data.cube.reset_rotation_buffer();

    app_data.stage = 0;
}

void set_stage_1(AppData& app_data)
{
    app_data.timer_time = 0.0f;

    Scrambles::RotationList* scramble = Scrambles::generateScramble(15);

    app_data.scramble_window_text = scramble->to_str();

    app_data.cube.apply_scramble(scramble);

    app_data.last_scramble_ptr = std::shared_ptr<Scrambles::RotationList>(scramble);

    app_data.timer_color = ImVec4(1.0, 1.0, 1.0, 1.0);

    app_data.ignore_space = true;
    app_data.ready_to_start = false;

    app_data.stage = 1;
}

void set_stage_2(AppData& app_data)
{
    app_data.scramble_color = ImVec4(1.0, 1.0, 1.0, 1.0);
    app_data.timer_color = ImVec4(1.0, 1.0, 1.0, 1.0);
    app_data.ready_to_start = false;
    app_data.stage = 2;
}

void update(AppData& app_data)
{
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    ImGuiIO& io = ImGui::GetIO();

    if (app_data.stage == 0)
    {
        if (io.KeysData[ImGuiKey_Space].DownDuration == 0.0f)
        {
            set_stage_1(app_data);
        }
    }
    else if (app_data.stage == 1)
    {
        if (!io.KeysData[ImGuiKey_Space].Down) {
            app_data.ignore_space = false;
            app_data.timer_color = ImVec4(1.0, 1.0, 1.0, 1.0);
            app_data.scramble_color = ImVec4(1.0, 1.0, 1.0, 1.0);
        }

        if (io.KeysData[ImGuiKey_Space].DownDuration >= 0.0f && !app_data.ignore_space) {
            app_data.timer_color = ImVec4(1.0, 0.8, 0.8, 1.0);
            app_data.scramble_color = ImVec4(1.0, 0.8, 0.8, 1.0);
        }

        if (io.KeysData[ImGuiKey_Space].DownDuration >= 0.5f && !app_data.ignore_space)
        {
            app_data.timer_time = 0.0f;
            app_data.scramble_color = ImVec4(1.0, 0.2, 0.2, 1.0);
            app_data.timer_color = ImVec4(1.0, 0.2, 0.2, 1.0);
            app_data.scramble_window_text = "release SPACE to start timer";
            app_data.ready_to_start = true;
            app_data.cube.percent_per_second = 1000000.0f;
        }

        if (!io.KeysData[ImGuiKey_Space].Down && app_data.ready_to_start)
        {
            set_stage_2(app_data);
        }
    }
    else if (app_data.stage == 2)
    {
        app_data.timer_time += io.DeltaTime;

        if (io.KeysData[ImGuiKey_Space].Down) {
            app_data.results.add_new(SingleResult(app_data.timer_time, app_data.last_scramble_ptr));
            set_stage_0(app_data);
        }
    }

    if (io.MouseReleased[0])
        app_data.cursor_rotate_cube = false;

    if (io.MouseDownDuration[0] == 0.0f && io.MousePos.x < display_w * 2.0 / 3.0 && io.MousePos.y < display_h * 2.0 / 3.0)
        app_data.cursor_rotate_cube = true;

    app_data.cube.camera.aspect_ratio = (display_w * 3.0 / 5.0) / (display_h * 2.0 / 3.0);
}

void imgui_window(AppData& app_data)
{
    {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(display_w * 3 / 5, display_h * 2 / 3));

        ImGui::Begin("CubeRenderWindow", NULL,
            ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoResize);

        if (ImGui::Button("Cube settings"))
            ImGui::OpenPopup("Cube Settings");

        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("Cube Settings", NULL,
            ImGuiWindowFlags_AlwaysAutoResize
            | ImGuiWindowFlags_NoMove))
        {
            app_data.cursor_rotate_cube = false;

            ImGui::Text("Scramble rotations/sec");
            ImGui::SameLine();
            ImGui::SliderFloat("##rotation_speed", &app_data.rotation_per_sec, 0.2, 5.0);
            ImGui::SameLine();
            if (ImGui::Button("Reset")) {
                app_data.rotation_per_sec = 1.2f;
            }
            app_data.cube.percent_per_second = app_data.rotation_per_sec;

            if (ImGui::Button("Reset camera")) {
                app_data.cube.camera.reset();
            }

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            ImGui::Separator();

            if (ImGui::Button("OK", ImVec2(120, 0)))
                ImGui::CloseCurrentPopup();

            ImGui::EndPopup();
        }

        ImGui::End();
    }

    {
        ImGui::SetNextWindowPos(ImVec2(display_w * 3 / 5, 0));
        ImGui::SetNextWindowSize(ImVec2(display_w * 2 / 5, display_h * 2 / 3));

        ImGui::Begin("ResultsWindow", NULL,
            ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoResize);

        app_data.results.show_table(app_data.table_font_big, app_data.table_font_small);

        ImGui::End();
    }

    {
        ImGui::PushFont(app_data.scramble_font);
        ImGui::SetNextWindowPos(ImVec2(0, display_h * 2 / 3));
        ImGui::SetNextWindowSize(ImVec2(display_w, display_h / 6));

        ImGui::Begin("ScrambleWindow", NULL,
            ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoResize);

        ImGui::PushFont(app_data.default_font);

        if (app_data.stage == 1)
            if (ImGui::Button("Back"))
                set_stage_0(app_data, true);

        ImGui::PopFont();

        if (app_data.stage != 2)
            TextColoredCentered(app_data.scramble_color, app_data.scramble_window_text);

        ImGui::End();
        ImGui::PopFont();
    }

    {
        ImGui::PushFont(app_data.timer_font);

        ImGui::SetNextWindowPos(ImVec2(0, display_h * 5 / 6));
        ImGui::SetNextWindowSize(ImVec2(display_w, display_h / 6));

        ImGui::Begin("TimerWindow", NULL,
            ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoResize);

        TextColoredCentered(app_data.timer_color, format_time(app_data.timer_time));

        ImGui::End();
        ImGui::PopFont();
    }

    ImGui::Render();
}

void render(AppData& app_data)
{
    ImVec4 clear_color = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, display_h / 3, display_w * 3 / 5, display_h * 2 / 3);
    glClear(GL_DEPTH_BUFFER_BIT);
    app_data.cube.render(program);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void body()
{
    AppData app_data = AppData();

    app_data.cube.reset();

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.06f, 0.06f, 0.06f, 0.0f));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Poll for and process events */
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiIO& io = ImGui::GetIO();

        update(app_data);

        imgui_window(app_data);
        
        if (io.MouseDown[0] && app_data.cursor_rotate_cube)
            app_data.cube.rotate_camera(3 * io.MouseDelta.x / display_w, 3 * io.MouseDelta.y / display_w);

        app_data.cube.update(io.DeltaTime);

        render(app_data);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }
}

int main(void)
{
    if (init() != 0)
        return -1;

    body();

    deinit();

    return 0;
}
