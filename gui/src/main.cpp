#define GUI
#include "Core/application.h"

#include <Engine.h>
#include <Window.h>
#include <TimeDef.h>

#include <imgui/imgui.h>
#include "imgui_impl_sdl_gl3.h"

#include "MainWindow.h"
#include <Core/Debug.h>
#include <Input/InputManager.h>
void menuBar() {
}

int main() {
    //fm::Window window(800, 600, "FML Engine");
    //fm::Engine engine;
    fm::Config config;
    config.name = "FML Engine";
    config.fpsWanted = 60;
    config.width = 800;
    config.height = 600;
    fm::Application app(config);
    app.Init();
    //app.Read();
    //engine.Init();
    fm::Window *window = app.GetWindow();
    fm::Engine *engine = app.GetEngine();

    IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplSdlGL3_Init(window->getWindow());
    
    MainWindow mainWindow(engine);


    //fm::Debug::log("INIT0");
    while(!window->isClosed())
    {
        while (SDL_PollEvent(&fm::InputManager::getInstance().getLastEvent()))
        {
            fm::InputManager::getInstance().processEvents();
            ImGui_ImplSdlGL3_ProcessEvent(&fm::InputManager::getInstance().getLastEvent());
        }

        app.Update();
        ImGui_ImplSdlGL3_NewFrame(window->getWindow());
        mainWindow.draw();

        ImGui::Render();
        ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
        window->swapBuffers();

    }
    ImGui_ImplSdlGL3_Shutdown();
    //app.Serialize();
    app.DeInit();
    return 0;
}
