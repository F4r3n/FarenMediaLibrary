#pragma once
#include "component.h"
#include "Core/Color.h"
#include <nlohmann/json_fwd.hpp>

namespace fmc {
class CPointLight {
    public:
        CPointLight();
        uint16_t GetType() const {return kPointLight;}

		bool Serialize(nlohmann::json& ioJson) const;
		bool Read(const nlohmann::json& inJSON);

        fm::Color color;
        float radius = 100;
        void Destroy();
private:
	std::string _name;
};
}
