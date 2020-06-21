#pragma once
#include <vector>
#include <string>
#include <nlohmann/json_fwd.hpp>
#include <ECS.h>
#include <memory>
#include "Core/Observer.h"
#include "FilePath.h"
namespace fm
{
    class GameObject;
}


namespace fm {

    class Scene : public Observable
	{
        public:
			enum class Event
			{
				CREATE_GAMEOBJECT,
				DELETE_GAMEOBJECT
			};
			using MapOfGameObjects = std::map<ecs::id, std::shared_ptr<fm::GameObject>>;

            Scene(const fm::FilePath &inPath);
			Scene(const std::string& inName);

            ~Scene();

			std::shared_ptr<fm::GameObject> CreateGameObject(bool defaultValue);
            void							Serialize(nlohmann::json &outJson);
            bool							Read(const nlohmann::json &inJson);

            void							Destroy();
            const std::string&				GetName() const{ return _name;}
			const MapOfGameObjects&			GetAllGameObjects() {return _gos;}
			void							SetStatusToGo(bool inStatus);
			void							ResetStatusGo();

			std::shared_ptr<fm::GameObject> GetGameObjectByID(ecs::id inID);

			void							DeleteGameObjectByID(ecs::id inID);

			void							Load();
			void							Save();
			const fm::FilePath&				GetPath() const { return _path; }
        private:
			MapOfGameObjects _gos;
			size_t			 _UniqueIDScene = 0;
            std::string		 _name;
			fm::FilePath	 _path;
			bool			 _isDirty = false;
    };
}
