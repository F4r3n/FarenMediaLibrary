#include "Components/CIdentity.h"
#include "nlohmann/json.hpp"
using namespace fmc;

CIdentity::~CIdentity()
{
}

CIdentity::CIdentity()
{
	_name = "Identity";
	_nameEntity = "";
}

bool CIdentity::Serialize(nlohmann::json &ioJson) const
{
	ioJson["name"] = _nameEntity;
	ioJson["active"] = _active;

    return true;
}
bool CIdentity::Read(const nlohmann::json &inJSON)
{
	_nameEntity = inJSON["name"];
	if(inJSON.contains("active"))
		_active = inJSON["active"];
    return true;
}

const std::string& CIdentity::GetName() const
{
    return _name;
}

