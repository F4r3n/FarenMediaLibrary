#include <iostream>
#include <Engine.h>
#include <Window.h>
#include <imgui.h>
#include <Time.h>

#include <CTransform.h>
#include <CMaterial.h>
#include <CText.h>
#include "imgui_impl_glfw_gl3.h"
int main() {
    fm::Window window(800, 600, "test");
    ImGui_ImplGlfwGL3_Init(window.getWindow(), true);
    fm::Engine engine;
    Entity* text = fm::Engine::createEntity();
    fmc::CTransform* tt =
        text->addComponent<fmc::CTransform>(new fmc::CTransform(fm::Vector2f(100, 100), fm::Vector2f(100, 100), 0, 3));
    fmc::CMaterial* m = text->addComponent<fmc::CMaterial>(new fmc::CMaterial(fm::Color(1, 1, 1, 1), false));
    fmc::CText* ctext = text->addComponent<fmc::CText>(new fmc::CText("salut", "dejavu", 800, 600));
    engine.init();
    bool show_test_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(114, 144, 154);

    while(!window.isClosed()) {

        window.update(60);
        ImGui_ImplGlfwGL3_NewFrame();

        // std::cout << 1 / fm::Time::dt << std::endl;
        // auto start = std::chrono::system_clock::now();
        engine.update(fm::Time::dt);
        // 1. Show a simple window
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called
        // "Debug"
        {
            static float f = 0.0f;
            ImGui::Text("Hello, world!");
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);
            if(ImGui::Button("Test Window"))
                show_test_window ^= 1;
            if(ImGui::Button("Another Window"))
                show_another_window ^= 1;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
        }

        // 2. Show another simple window, this time using an explicit Begin/End pair
        if(show_another_window) {
            
            ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello");
            ImGui::End();
        }

        // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
        if(show_test_window) {
            ImGui::SetNextWindowPos(ImVec2(100, 20), ImGuiSetCond_FirstUseEver);
            ImGui::ShowTestWindow(&show_test_window);
        }
        
        
        ImGui::Render();
        window.swapBuffers();
    }
     ImGui_ImplGlfwGL3_Shutdown();
    return 0;
}
