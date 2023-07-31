#include "Components/CIdentity.h"
#include <ECS.h>
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
    return true;
}
bool CIdentity::Read(const nlohmann::json &inJSON)
{
	_nameEntity = inJSON["name"];
    return true;
}

const std::string& CIdentity::GetName() const
{
    return _name;
}

