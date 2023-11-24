#pragma once
#include <vector>
#include <string>
#include <nlohmann/json_fwd.hpp>
#include <memory>
#include "Core/Observer.h"
#include "FilePath.h"
#include <entt/entt.hpp>
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
			using MapOfGameObjects = std::map<size_t, std::shared_ptr<fm::GameObject>>;

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

			std::shared_ptr<fm::GameObject> GetGameObjectByID(size_t inID);

			void							DeleteGameObjectByID(size_t inID);

			void							Load();
			void							Save();
			const fm::FilePath&				GetPath() const { return _path; }
			entt::registry& GetRegistry() { return _registry; }

			void Start();
			void Stop();
			void Update(float dt);

        private:
			MapOfGameObjects _gos;
			size_t			 _UniqueIDScene = 0;
            std::string		 _name;
			fm::FilePath	 _path;
			bool			 _isDirty = false;
			entt::registry	 _registry;

    };
}
