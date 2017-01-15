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
    fm::Window window(800, 600, "FML Engine");
    ImGui_ImplGlfwGL3_Init(window.getWindow(), true);
    fm::Engine engine;

    engine.init();

    
    MainWindow mainWindow(&engine);

    while(!window.isClosed()) {

        window.update(60);
        ImGui_ImplGlfwGL3_NewFrame();

        engine.update(fm::Time::dt);

        mainWindow.draw();
        
        ImGui::Render();
        window.swapBuffers();
    }
    ImGui_ImplGlfwGL3_Shutdown();
    return 0;
}
