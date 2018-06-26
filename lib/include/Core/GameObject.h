#pragma once
#include <ECS.h>
#include <nlohmann/json_fwd.hpp>
#include "Components/component.h"

namespace fm
{
class Scene;
}

namespace fm {

    class GameObject {
        public:
            GameObject();
            ~GameObject();
            GameObject* create();
            GameObject* create(Scene *s);
            bool IsActive()
            {
                return _entity != nullptr && _entity->active;
            }

            void destroy() {
                _entity->destroy();
            }

            template <typename T> T* add(Component<T> *c)
            {
                return _entity->add<T>(c);
            }

            template <typename T, typename ...Args> T* addComponent(Args&&...args)
            {
                return _entity->addComponent<T>(args...);
            }
            template <typename T> T* add()
            {
                return _entity->add<T>();
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

            void Serialize(nlohmann::json &outResult);


            bool Read(const nlohmann::json &inJson);

            

        private:
            Entity* _entity = nullptr;

    };

    class GameObjectHelper
    {
        public:
            static GameObject* create()
            {
                return (new GameObject())->create();
            }
            static GameObject* create(Scene *scene)
            {
                return (new GameObject())->create(scene);
            }
            template <typename T>
            static void Destroy(size_t ID)
            {
                EntityManager::get().removeComponent<T>(ID);
            }

    };
}
