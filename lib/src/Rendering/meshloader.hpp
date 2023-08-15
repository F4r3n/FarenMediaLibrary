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

namespace fm
{
class MeshLoader
{
    public:
        static std::optional<std::shared_ptr<fm::Model>> Load(const fm::FilePath &inFilePath, const std::string &inObjectName);
    private:
        static void ProcessMesh(Model *inModel, aiMesh *mesh, const aiScene *scene);
        static void ProcessNode(Model *inModel, aiNode *node, const aiScene *scene);

};
}

#endif // MESHLOADER_HPP

