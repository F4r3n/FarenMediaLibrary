#pragma once
#include <vector>
#include <string>
#include <nlohmann/json_fwd.hpp>
#include <ECS.h>
namespace fm {
    class GameObject;
}



namespace fm {

    class Scene {
        public:

			using MapOfGameObjects = std::map<ecs::id, fm::GameObject*>;

            Scene(const std::string &name);
            ~Scene();
            void AddGameObject(GameObject *e);
            void Serialize(nlohmann::json &outJson);
            bool Read(const nlohmann::json &inJson);

            void destroy();
            const std::string& getName() const{ return _name;}
			const MapOfGameObjects& getAllGameObjects() {return _gos;}
			void SetStatusToGo(bool inStatus);
			void ResetStatusGo();

			fm::GameObject* GetGameObjectByID(ecs::id inID);

			void DeleteGameObjectByID(ecs::id inID);
        private:
			MapOfGameObjects _gos;
            std::string _name;
            //Light setting
    };
}
