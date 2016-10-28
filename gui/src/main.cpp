#define GUI
#include <iostream>
#include <Engine.h>
#include <Window.h>
#include <Time.h>

#include <imgui.h>

#include "MainWindow.h"

void menuBar() {
}

int main() {
    fm::Window window(800, 600, "test");
    ImGui_ImplGlfwGL3_Init(window.getWindow(), true);
    fm::Engine engine;

    engine.init();


    Entity* light = fm::Engine::createEntity();
    fmc::CPointLight* p = light->addComponent<fmc::CPointLight>();
    p->color = fm::Color(1, 1, 1, 1);

    light->addComponent<fmc::CTransform>(new fmc::CTransform(fm::Vector2f(100, 50), fm::Vector2f(20, 20), 0, 1));
    light->addComponent<fmc::CMaterial>();
    
    MainWindow mainWindow(&engine);

    while(!window.isClosed()) {

        window.update(60);
        ImGui_ImplGlfwGL3_NewFrame();

        // std::cout << 1 / fm::Time::dt << std::endl;
        // auto start = std::chrono::system_clock::now();
        engine.update(fm::Time::dt);
        // 1. Show a simple window
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called
        // "Debug"
        mainWindow.draw();
        
        ImGui::Render();
        window.swapBuffers();
    }
    ImGui_ImplGlfwGL3_Shutdown();
    return 0;
}
