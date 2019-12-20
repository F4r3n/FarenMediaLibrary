#pragma once
#include "component.h"
#include <string>
#include "Core/Math/Vector2.h"

#include "Core/Color.h"
#include "Core/Bounds.h"

namespace fm
{
	class Model;
}

namespace fmc {


class CMesh : public FMComponent<CMesh> {

    public:
        CMesh(std::string type);
		CMesh();

        ~CMesh();

        bool Serialize(json &ioJson) const override;
        bool Read(const json &inJSON) override;
        const std::string& GetName() const override;
		uint16_t GetType() const override {return KMesh;}
        void Destroy() override;


        void SetType(const std::string &type);
        bool IsmodelReady();
        const std::string& GetModelType() const {return _type;}
        void SetModelType(const std::string &type) {_type = type;}
        fm::Model* model = nullptr;

    private:
        std::string _type;
};
}
