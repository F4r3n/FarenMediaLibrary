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
            GameObject* create(std::shared_ptr<Scene> s, bool defaultValue);

            bool IsActive() const
            {
                return _entity != nullptr && _entity->active;
            }

            void destroy()
			{
                _entity->destroy();
            }

			void activate(bool value)
			{
				_entity->active = value;
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

            template <typename T> T* get() const
            {
                return _entity->get<T>();
            }

            template <typename T> bool has() const
            {
                return _entity->has<T>();
            }

            template <typename T> bool remove()
            {
                return _entity->remove<T>();
            }

			std::vector<BaseComponent*> getAllComponents() const;

            inline ecs::id getID() {return _entity->ID;}

            void Serialize(nlohmann::json &outResult) const;


            bool Read(const nlohmann::json &inJson);
			void SetStatus(bool inStatus);
			void ResetStatus();
			void SetName(const std::string &inName);
			const std::string& GetName() const;
        private:
            Entity* _entity = nullptr;
			bool	_oldStatus;
			static size_t _counter;
    };

    class GameObjectHelper
    {
        public:

            static GameObject* create(std::shared_ptr<Scene> scene, bool defaultValues)
            {
                return (new GameObject())->create(scene, defaultValues);
            }


            template <typename T>
            static void Destroy(ecs::id ID)
            {
                EntityManager::get().removeComponent<T>(ID);
            }

    };
}
