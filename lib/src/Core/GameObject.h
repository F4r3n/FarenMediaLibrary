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
                return _active;
            }

            void destroy()
			{
				_GetEntity().destroy();
            }

			void activate(bool value)
			{
				_active = value;
			}

            template <typename T> T* add(Component<T> *c)
            {
                return _GetEntity().addComponent<T>(c);
            }

            template <typename T, typename ...Args> T* addComponent(Args&&...args)
            {
                return _GetEntity().addComponent<T>(args...);
            }
            template <typename T> T* add()
            {
                return _GetEntity().addComponent<T>();
            }

            template <typename T> T* get() const
            {
                return _GetEntity().get<T>();
            }

            template <typename T> bool has() const
            {
                return _GetEntity().has<T>();
            }

            template <typename T> bool remove()
            {
                return _GetEntity().remove<T>();
            }

			std::vector<BaseComponent*> getAllComponents() const;

            inline Entity::Id getID() {return _GetEntity().id();}

            void Serialize(nlohmann::json &outResult) const;


            bool Read(const nlohmann::json &inJson);
			void SetStatus(bool inStatus);
			void ResetStatus();
			void SetName(const std::string &inName);
			const std::string& GetName() const;

			void SetOrder(size_t inOrder) { _order = inOrder; }
			size_t GetOrder() const { return _order; }
        private:
			Entity _GetEntity() const;
            //Entity* _entity = nullptr;
			Entity::Id _id = Entity::INVALID;
			bool	_oldStatus;
			size_t  _order = 0;
			bool _active = true;
    };

}
