#ifndef MESHLOADER_HPP
#define MESHLOADER_HPP

#include <string>

struct aiMesh;
struct aiScene;
struct aiNode;
namespace fm
{
    class Model;
}

namespace fm
{
class MeshLoader
{
    public:
        static bool Load(const std::string &inFileName, Model *outModel, const std::string &inObjectName);
    private:
        static void ProcessMesh(Model *inModel, aiMesh *mesh, const aiScene *scene);
        static void ProcessNode(Model *inModel, aiNode *node, const aiScene *scene);

};
}

#endif // MESHLOADER_HPP

