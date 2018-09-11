#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
namespace fm
{
class Window;
class Engine;
}

namespace fm
{

struct Config
{
    std::string name = "Empty";
    size_t width = 800;
    size_t height = 600;
    size_t fpsWanted = 60;
};

class Application
{
    public:
        Application(const Config &inConfig);
        ~Application();

        bool Serialize();
        bool Read();

        void Start();
        void Init();
        void DeInit();

        void Update();

        fm::Window* GetWindow() const;
        fm::Engine* GetEngine() const;
    private:
        fm::Engine *_engine;
        fm::Window *_window;
        fm::Config _currentConfig;
};
}

#endif // APPLICATION_H

