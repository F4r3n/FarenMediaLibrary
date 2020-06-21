#pragma once 
#include <map>
#include <memory>
#include <string>
namespace fm
{
	class Scene;
	class FilePath;
}



namespace fm 
{
    class SceneManager 
	{
        public:
            SceneManager();
			~SceneManager();

            void							SetCurrentScene(const std::string &name, bool isPrivate);
            std::shared_ptr<fm::Scene>		AddNewScene(const fm::FilePath &inPath);
			void							AddScene(std::shared_ptr<fm::Scene> inScene);

			std::shared_ptr<Scene>			GetScene(const std::string &name);
            inline std::shared_ptr<Scene>	GetCurrentScene() const {
                return _currentScene;
            }
            void							Serialize(nlohmann::json &outjson);
			void							SerializeCurrentScene(nlohmann::json &outjson);

            bool							Read(const nlohmann::json &injson);

			bool							SerializePrivate(const std::string &inName, nlohmann::json &outjson);
			bool							ReadPrivate(const std::string &inName, const nlohmann::json &injson);

			void							ClearAll(bool clearPrivate);

			std::shared_ptr<fm::Scene>		AddPrivateScene(const std::string &inName);
			bool							ClearScene(const std::string &inName, bool isPrivate);
			std::shared_ptr<Scene>			RenameScene(std::shared_ptr<Scene> inCurrentScene, const fm::FilePath& inPath);
        private:
            std::shared_ptr<Scene>							_currentScene = nullptr;
            std::map<std::string, std::shared_ptr<Scene>>	_scenes;
			std::vector<std::shared_ptr<fm::Scene>>			_privateScenes;
    };

}
