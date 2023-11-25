#pragma once
#include <nlohmann/json_fwd.hpp>
#include "Components/component.h"
#include <entt/entt.hpp>
#include "Scene.h"
#include <set>
#include "Core/GameObjectType.hpp"
namespace fm
{
	class Scene;
}

namespace fmc {
	class CIdentity;
}


namespace fm {

	class GameObject {
	public:
		GameObject(entt::handle inHandle);
		~GameObject();

		void destroy()
		{
			//_GetEntity().destroy();
		}

		void activate(bool value);


		bool IsActive() const;


		template <typename T, typename ...Args> T& addComponent(Args&&...args)
		{
			return _entity.emplace<T>(std::forward<Args>(args)...);
		}
		template <typename T> T& add() const
		{
			return _entity.emplace<T>();
		}

		template <typename T, typename ...Args> T& emplaceOrReplace(Args&&...args) const
		{
			return _entity.emplace_or_replace<T>(std::forward<Args>(args)...);
		}

		template <typename T> T& get() const
		{
			return _entity.get<T>();
		}

		template <typename T> T* try_get() const
		{
			return _entity.try_get<T>();
		}

		template <typename T> bool has() const
		{
			return _entity.all_of<T>();
		}

		template <typename T> bool remove()
		{
			return _entity.remove<T>();
		}

		//template <typename ...T>
		//std::vector<BaseComponent*> getAllComponents() const
		//{
		//	return _entity.<T>(_entity);
		//}


		//std::vector<BaseComponent*> getAllComponents() const;

		inline entt::handle GetEntity() { return _entity; }

		void Serialize(nlohmann::json& outResult) const;


		bool Read(const nlohmann::json& inJson);
		void SetStatus(bool inStatus);
		void ResetStatus();
		void SetName(const std::string& inName);
		const std::string& GetName() const;

		void SetOrder(size_t inOrder) { _order = inOrder; }
		size_t GetOrder() const { return _order; }
		GameObjectID_t GetID() const { return _currentID; }
	private:
		bool _active = true;
		fmc::CIdentity& _GetIdentity() const;
		entt::handle _entity;
		bool	_oldStatus;
		size_t  _order = 0;
		GameObjectID_t _currentID = 0;
		static inline GameObjectID_t _ID = 0;

	};

}
