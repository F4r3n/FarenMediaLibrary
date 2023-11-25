#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Core/Observer.h"
#include "Rendering/GraphicsAPI.h"
#include <memory>
#include "Core/FilePath.h"
#include <array>
namespace fm
{
	class Window;
	class Engine;
	class Scene;
	class SceneManager;
	class FilePath;
}

namespace fmc
{
	class CCamera;
	class CTransform;
}

namespace fm
{
	struct Config
	{
		std::string name = "Empty";
		uint32_t		width = 800;
		uint32_t		height = 600;
		uint32_t		fpsWanted = 60;
		bool			standAlone = false;
		RENDERING_MODE	graphicAPI = RENDERING_MODE_VULKAN;
		size_t			windowFlag = 0;

		fm::Folder	userDirectory;
		fm::Folder	internalResourcesDirectory;
	};

	struct ProjectSettings
	{
		std::string name = "Project";
		fm::FilePath path;
		std::string resourcesFolder = "Resources";
	};

	//Could be drastically optimized, but not needed
	template <typename T, size_t P>
	class CircularBuffer
	{
	public:

		CircularBuffer()
		{
		}
		void Push(T& t)
		{
			size_t nextLimit = _currentLimit + 1;
			if (_maxLimit < nextLimit)
			{
				_list.push_back(t);
				_currentLimit = nextLimit;
				_index = nextLimit;
			}
			else
			{
				if (_index + 1 > _maxLimit)
					_index = 0;
				_list[_index] = t;
			}
		}


		size_t Size()
		{
			return _currentLimit;
		}
		bool IsEmpty()
		{
			return _currentLimit != 0;
		}

		const T& operator[](size_t index)
		{
			return _list[index];
		}
	private:
		size_t _index = 0;
		size_t _maxLimit = P;
		size_t _currentLimit = 0;

		std::vector<T> _list;
	};


	using Windows = std::array<std::shared_ptr<fm::Window>, (int)GRAPHIC_API::LAST>;
	class Application
	{
	public:

		~Application();
		Application();
		inline static Application& Get()
		{
			static Application app;
			return app;
		}

		bool						Serialize() const;
		bool						SerializeCurrentScene() const;

		bool						Read();

		void						Start();
		void						Stop();
		void						SetConfig(const Config& inConfig);
		void						Init();
		void						InitSystems();
		void						DeInit();
		void						LoadInternalResources();
		void						Update();
		void						UpdateScene(std::shared_ptr<fm::Scene> inScene);
		void						DrawScene(std::shared_ptr<fm::Scene> inScene, fmc::CCamera& inCamera, fmc::CTransform& inTransform);

		fm::Window* GetWindow(GRAPHIC_API api = GRAPHIC_API::OPENGL) const;

		const fm::Folder& GetUserDirectory() const;
		const fm::Folder& GetInternalResources() const;

		const fm::Config& GetCurrentConfig() const;
		bool						IsRunning() const;

		void						SetProjectName(const std::string& inName);
		std::shared_ptr<fm::Scene>	GetScene(const std::string& inName) const;
		const std::string& GetCurrentSceneName() const;
		std::shared_ptr<fm::Scene>	GetCurrentScene() const;
		void						LoadProject(const fm::Folder& inPath);
		void						SetUserDirectory(const fm::Folder& inPath);
		std::shared_ptr<fm::Scene>	LoadScene(const fm::FilePath& inPath);


		std::shared_ptr<fm::Scene>	AddNewScene(const fm::FilePath& inPath);
		void						SetCurrentScene(const std::string& name, bool isPrivate);
		std::shared_ptr<fm::Scene>	AddPrivateScene(const std::string& inName);
		bool						ClearScene(const std::string& inName, bool isPrivate, bool remove = false);
		std::shared_ptr<Scene>		RenameScene(std::shared_ptr<Scene> inCurrentScene, const fm::FilePath& inPath);
		void						SerializeCurrentScene(nlohmann::json& outjson);
		void						SwapBuffers();
	private:
		std::unique_ptr<fm::SceneManager>	_sceneManager;

		std::unique_ptr<fm::Engine>			_engine;
		Windows								_window;
		fm::Config							_currentConfig;
		std::string							_nameLastScene;
	};

}


