#define GUI
#include <iostream>
#include <Engine.h>
#include <Window.h>
#include <Time.h>

#include <imgui.h>
#include "imgui_impl_sdl_gl3.h"

#include "MainWindow.h"
#include <Core/Debug.h>
#include <Input/InputManager.h>
void menuBar() {
}

int main() {
    fm::Window window(800, 600, "FML Engine");
    fm::Engine engine;
    engine.init();
    ImGui_ImplSdlGL3_Init(window.getWindow());

    
    MainWindow mainWindow(&engine);

    while(!window.isClosed()) {
        while (SDL_PollEvent(&fm::InputManager::getInstance().getLastEvent()))
        {
            fm::InputManager::getInstance().processEvents();
            ImGui_ImplSdlGL3_ProcessEvent(&fm::InputManager::getInstance().getLastEvent());
        }
        window.update(60, false);
        ImGui_ImplSdlGL3_NewFrame(window.getWindow());
        
        engine.update(fm::Time::dt);

        mainWindow.draw();

        ImGui::Render();
        window.swapBuffers();
    }
    ImGui_ImplSdlGL3_Shutdown();
    return 0;
}
