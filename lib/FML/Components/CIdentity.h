#pragma once
#include "component.h"
#include <string>
#include "nlohmann/json_fwd.hpp"
#include "Core/GameObjectType.hpp"
namespace fmc {


class CIdentity
{
    public:
		CIdentity();

        ~CIdentity();

        bool Serialize(nlohmann::json &ioJson) const;
        bool Read(const nlohmann::json &inJSON);
        const std::string& GetName() const;
		uint16_t GetType() const {return kIdentity;}

		const std::string& GetNameEntity() const { return _nameEntity; }
		void SetNameEntity(const std::string &inName) { _nameEntity = inName; }

		bool IsActive() const { return _active; }
		void SetActive(bool value) { _active = value; }

		fm::GameObjectID_t GetID() const { return _gameObjectID; }
		void SetID(fm::GameObjectID_t value) { _gameObjectID = value; }
    private:
		fm::GameObjectID_t	_gameObjectID = 0;

        std::string _nameEntity;
		bool		_active = true;
private:
	std::string _name;
};
}
