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
                return _entity->addComponent<T>(c);
            }

            template <typename T, typename ...Args> T* addComponent(Args&&...args)
            {
                return _entity->addComponent<T>(args...);
            }
            template <typename T> T* add()
            {
                return _entity->addComponent<T>();
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

			void SetOrder(size_t inOrder) { _order = inOrder; }
			size_t GetOrder() const { return _order; }
        private:
            Entity* _entity = nullptr;
			bool	_oldStatus;
			size_t  _order = 0;
    };

}
