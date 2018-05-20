#pragma once
#include "component.h"
#include <string>
#include <vector>
#include "Core/Math/Vector2.h"

#include <Core/Config.h>
#include "Core/Color.h"
#include "Core/Bounds.h"

#include <Rendering/Model.hpp>

namespace fmc {
enum SHAPE { RECTANGLE, CIRCLE, LAST_SHAPE };
struct Vertex {
        fm::math::Vector2f position;
        fm::math::Vector2f uv;
};

class CMesh : public FMComponent<CMesh> {

    public:
        CMesh(std::string type);
        CMesh();
        ~CMesh();

        bool Serialize(json &ioJson) const override;
        bool Read(const json &inJSON) override;
        const std::string& GetName() const override;
        virtual size_t GetType() const {return KMesh;}

        static const std::string name;


        void ComputeBoundingSize();
        void SetType(const std::string &type);
        fm::Bounds bounds;
        bool IsmodelReady();
        const std::string& GetModelType() const {return type;}
        void SetModelType(const std::string &type) {this->type = type;}
        fm::Model* model = nullptr;

    private:
        bool created = false;
        std::string type = "Quad";
};
}
