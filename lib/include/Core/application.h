#ifndef APPLICATION_H
#define APPLICATION_H
#include "Core/FilePath.h"
#include <list>
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

	//Could be drastically optimized, but not needed
	template <typename T, size_t P>
	class CircularBuffer
	{
	public:
		
		CircularBuffer()
		{
		}
		void Push(T & t)
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
		const fm::FilePath& GetUserDirectory() const;

		void SetProjectName(const std::string &inName);
		void RegisterCurrentConfig();
		void GetLastConfigs(std::vector<fm::Config> &outConfig);
    private:
		Application();
		fm::CircularBuffer<fm::Config,10> _lastConfigsUsed;

        fm::Engine *_engine;
        fm::Window *_window;
        fm::Config _currentConfig;
		
};
}

#endif // APPLICATION_H

