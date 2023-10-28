#ifndef MESHLOADER_HPP
#define MESHLOADER_HPP

#include <string>
#include <optional>
#include <memory>
struct aiMesh;
struct aiScene;
struct aiNode;
namespace fm
{
    class Model;
	class FilePath;
}

namespace tinyobj
{
	class ObjReader;
}

namespace fm
{
class MeshLoader
{
    public:
        static std::optional<std::shared_ptr<fm::Model>> Load(const fm::FilePath &inFilePath, const std::string &inObjectName);
    private:
#if WITH_ASSIMP
        static void ProcessMesh(Model *inModel, aiMesh *mesh, const aiScene *scene);
		static void ProcessNode(Model* inModel, aiNode* node, const aiScene* scene);
#endif
		static void ProcessMesh(Model* inModel, const tinyobj::ObjReader& inReader);


};
}

#endif // MESHLOADER_HPP

