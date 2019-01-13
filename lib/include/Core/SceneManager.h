#pragma once 
#include <map>
#include <memory>
namespace fm
{
class Scene;
}



namespace fm {
    class SceneManager {
        public:
            SceneManager();
            void setCurrentScene(const std::string &name);
            void addScene(Scene *scene);
            Scene* getScene(const std::string &name);
            static SceneManager& get();
            inline Scene* getCurrentScene() const {
                return _currentScene;
            }
            void Serialize(nlohmann::json &outjson);
            bool Read(const nlohmann::json &injson);

			bool SerializePrivate(const std::string &inName, nlohmann::json &outjson);
			bool ReadPrivate(const std::string &inName, const nlohmann::json &injson);

			void ClearAllPublic();

			std::shared_ptr<fm::Scene> AddPrivateScene(const std::string &inName);
			bool ClearScene(const std::string &inName);
        private:
            ~SceneManager();
            Scene* _currentScene = nullptr;
            static SceneManager _instance;
            std::map<std::string, Scene*> _scenes;
			std::vector<std::shared_ptr<fm::Scene>> _privateScenes;
    };

}
