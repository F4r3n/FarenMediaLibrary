#pragma once
#include "component.h"
#include <string>
#include "nlohmann/json_fwd.hpp"
namespace fmc {


class CIdentity : public FMComponent<CIdentity> {

    public:
		CIdentity();

        ~CIdentity();

        bool Serialize(nlohmann::json &ioJson) const override;
        bool Read(const nlohmann::json &inJSON) override;
        const std::string& GetName() const override;
		uint16_t GetType() const override {return kIdentity;}
        void Destroy() override;

		const std::string& GetNameEntity() const { return _nameEntity; }
		void SetNameEntity(const std::string &inName) { _nameEntity = inName; }
    private:
        std::string _nameEntity;
};
}
