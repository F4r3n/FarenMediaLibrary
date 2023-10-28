#pragma once
#include "component.h"
#include "Core/Color.h"
#include "Core/Math/Vector2.h"
#include <nlohmann/json_fwd.hpp>

namespace fmc {
class CPointLight : public FMComponent<CPointLight> {
    public:
        CPointLight();
        virtual uint16_t GetType() const override {return kPointLight;}

		bool Serialize(nlohmann::json& ioJson) const override;
		bool Read(const nlohmann::json& inJSON) override;

        fm::Color color;
        float radius = 100;
        void Destroy();

};
}
