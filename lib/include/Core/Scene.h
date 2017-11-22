#pragma once
#include <vector>
#include <string>

namespace fm {
    class GameObject;
}

namespace fm {

    class Scene {
        public:
            Scene(const std::string &name);
            ~Scene();
            void addEntity(GameObject *e);
            void serialize();
            void destroy();
            const std::string& getName() const{ return _name;}
            std::vector<fm::GameObject*> getAllGameObjects() {return _entities;}
        private:
            std::vector<GameObject*> _entities;
            std::string _name;
            
            //Light setting
    };
}
