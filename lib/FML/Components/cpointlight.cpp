#include "Components/CPointLight.h"
#include <EntityManager.h>
#include <nlohmann/json.hpp>
using namespace fmc;

namespace Keys
{
	const std::string color("color");
	const std::string radius("radius");
}

CPointLight::CPointLight()
{
    _name = "PointLight";
}



bool CPointLight::Serialize(nlohmann::json& ioJson) const
{
	ioJson[Keys::color] = color;
	ioJson[Keys::radius] = radius;

	return true;
}


bool CPointLight::Read(const nlohmann::json& inJSON)
{

	color = inJSON[Keys::color];
	radius = inJSON[Keys::radius];

	return true;
}

