#define GUI
#include <iostream>
#include <Engine.h>
#include <Window.h>
#include <Time.h>

#include <imgui/imgui.h>
#include "imgui_impl_sdl_gl3.h"

#include "MainWindow.h"
#include <Core/Debug.h>
#include <Input/InputManager.h>
#include "Core/application.h"
void menuBar() {
}

int main() {
    fm::Window window(800, 600, "FML Engine");
    fm::Engine engine;
    fm::Application app;
    //app.Read();
    engine.Init();


    IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplSdlGL3_Init(window.getWindow());
    
    MainWindow mainWindow(&engine);


    //fm::Debug::log("INIT0");
    while(!window.isClosed()) {

        while (SDL_PollEvent(&fm::InputManager::getInstance().getLastEvent()))
        {
            fm::InputManager::getInstance().processEvents();
            ImGui_ImplSdlGL3_ProcessEvent(&fm::InputManager::getInstance().getLastEvent());
        }

        window.update(120, false);
        ImGui_ImplSdlGL3_NewFrame(window.getWindow());

        engine.Update(fm::Time::dt);

        mainWindow.draw();

        ImGui::Render();
        ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
        window.swapBuffers();

    }
    ImGui_ImplSdlGL3_Shutdown();
    //app.Serialize();

    return 0;
}
