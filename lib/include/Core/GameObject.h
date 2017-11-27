#pragma once
#include <ECS.h>
#include "Core/SceneManager.h"
namespace fm {

    class GameObject {
        public:
            GameObject();
            ~GameObject();
            GameObject* create();

            void destroy() {
                _entity->destroy();
            }

            template <typename T> T* addComponent(Component<T> *c = new T())
            {
                return _entity->addComponent<T>(c);
            }
            template <typename T> T* add()
            {
                return _entity->addComponent<T>(new T());
            }

            template <typename T> T* get()
            {
                return _entity->get<T>();
            }

            template <typename T> bool has()
            {
                return _entity->has<T>();
            }

            template <typename T> bool remove()
            {
                return _entity->remove<T>();
            }

            std::vector<BaseComponent*> getAllComponents() {
                return _entity->getAllComponents();
            }

            inline size_t getID() {return _entity->ID;} 
            std::string name;
            

        private:
            Entity* _entity = nullptr;

    };
    class GameObjectHelper {
        public:
            static GameObject* create() {
                return (new GameObject())->create();
            }
    };
}
