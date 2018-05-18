#pragma once 
#include <map>
#include "Core/Scene.h"
#include "json.hpp"
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
        private:
            ~SceneManager();
            Scene* _currentScene = nullptr;
            static SceneManager _instance;
            std::map<std::string, Scene*> _scenes;

    };

}
