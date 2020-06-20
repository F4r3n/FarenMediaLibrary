#pragma once
#include <vector>
#include <string>
#include <nlohmann/json_fwd.hpp>
#include <ECS.h>
#include <memory>
#include "Core/Observer.h"
namespace fm {
    class GameObject;
}


namespace fm {

    class Scene : public Observable {
        public:
			enum class Event
			{
				CREATE_GAMEOBJECT,
				DELETE_GAMEOBJECT
			};
			using MapOfGameObjects = std::map<ecs::id, std::shared_ptr<fm::GameObject>>;

            Scene(const std::string &name);
            ~Scene();
			std::shared_ptr<fm::GameObject> CreateGameObject(bool defaultValue);
            void Serialize(nlohmann::json &outJson);
            bool Read(const nlohmann::json &inJson);

            void destroy();
            const std::string& getName() const{ return _name;}
			const MapOfGameObjects& getAllGameObjects() {return _gos;}
			void SetStatusToGo(bool inStatus);
			void ResetStatusGo();

			std::shared_ptr<fm::GameObject> GetGameObjectByID(ecs::id inID);

			void DeleteGameObjectByID(ecs::id inID);
        private:
			MapOfGameObjects _gos;
			size_t			 _UniqueIDScene = 0;
            std::string _name;
            //Light setting
    };
}
