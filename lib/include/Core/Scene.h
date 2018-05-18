#pragma once
#include <vector>
#include <string>
#include <json.hpp>
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
            void destroy();
            const std::string& getName() const{ return _name;}
            std::vector<fm::GameObject*> getAllGameObjects() {return _entities;}
        private:
            std::vector<GameObject*> _entities;
            std::string _name;
            
            //Light setting
    };
}
