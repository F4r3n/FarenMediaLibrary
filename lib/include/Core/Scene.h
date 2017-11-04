#pragma once
#include <ECS.h>
#include <vector>
#include <string>
namespace fm {
    
    class Scene {
    public:
        Scene(const std::string &name);
        ~Scene();
        void addEntity(Entity *e);
        void serialize();
    private:
        std::vector<Entity*> _entities;
        std::string _name;
        //Light setting
    };
}