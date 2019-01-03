#pragma once
#include <vector>
#include <string>
#include <nlohmann/json_fwd.hpp>
namespace fm {
    class GameObject;
}



namespace fm {

    class Scene {
        public:
            Scene(const std::string &name);
            ~Scene();
            void AddGameObject(GameObject *e);
            void Serialize(nlohmann::json &outJson);
            bool Read(const nlohmann::json &inJson);

            void destroy();
            const std::string& getName() const{ return _name;}
            GameObject* GetGameObject(size_t id) const;
            std::vector<fm::GameObject*> getAllGameObjects() {return _gos;}
        private:
            std::vector<GameObject*> _gos;
            std::string _name;
            
            //Light setting
    };
}
