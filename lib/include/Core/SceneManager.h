#pragma once 
#include <map>

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
			void SerializeEditor(nlohmann::json &outjson);
            bool Read(const nlohmann::json &injson);
			bool ReadEditor(const nlohmann::json &injson);
			void Clear(bool clearEditor);

			inline Scene* GetEditorScene() {
				return _editorScene;
			};

			void InitEditorScene();
        private:
            ~SceneManager();
            Scene* _currentScene = nullptr;
            static SceneManager _instance;
            std::map<std::string, Scene*> _scenes;
			Scene* _editorScene = nullptr;

    };

}
