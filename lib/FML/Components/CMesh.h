#pragma once
#include "component.h"
#include <string>
#include "Core/Math/Vector2.h"

#include "Core/Color.h"
#include "Core/Bounds.h"
#include "Core/FilePath.h"
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

        bool Serialize(nlohmann::json &ioJson) const override;
        bool Read(const nlohmann::json &inJSON) override;
        const std::string& GetName() const override;
		uint16_t GetType() const override {return KMesh;}

		fm::FilePath GetModelPath() const { return _modelPath; }
		void SetModelPath(const fm::FilePath& inPath);
		std::shared_ptr<fm::Model>	GetModel() const { return _model; }
    private:
		fm::FilePath _modelPath;
		std::shared_ptr<fm::Model> _model = nullptr;
};
}
