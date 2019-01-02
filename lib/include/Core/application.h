#ifndef APPLICATION_H
#define APPLICATION_H
#include "Core/FilePath.h"
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
	fm::FilePath userDirectory;
};

class Application
{
    public:
        ~Application();

		static Application& Get()
		{
			static Application app;
			return app;
		}

        bool Serialize(bool serializeEditor);
        bool Read();

        void Start();
		void SetConfig(const Config &inConfig);
        void Init();
        void DeInit();

        void Update();

        fm::Window* GetWindow() const;
        fm::Engine* GetEngine() const;

		void SetUserDirectory(const fm::FilePath &inPath);
		const fm::FilePath& GetUserDirectory();

		void SetProjectName(const std::string &inName);
    private:
		Application();

        fm::Engine *_engine;
        fm::Window *_window;
        fm::Config _currentConfig;
		
};
}

#endif // APPLICATION_H

