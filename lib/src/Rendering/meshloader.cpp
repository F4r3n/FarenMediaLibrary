#include "Rendering/meshloader.hpp"
#include <Rendering/Model.hpp>
#include <Rendering/Mesh.hpp>
#include <Core/Debug.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace fm;
using namespace rendering;

bool MeshLoader::Load(const std::string &inFileName, Model *outModel, const std::string &inObjectName)
{
    outModel = new Model(inObjectName);

    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(inFileName, aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        DEBUG_ERROR("Error import object");
        return false;
    }

    ProcessNode(outModel, scene->mRootNode, scene);


    return true;
}

void MeshLoader::ProcessMesh(Model *inModel, aiMesh *mesh, const aiScene *scene)
{
    fm::rendering::MeshContainer* meshContainer = new fm::rendering::MeshContainer();

    for(size_t i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;

        if(mesh->HasPositions())
        {
            vertex.position.x = mesh->mVertices[i].x;
            vertex.position.y = mesh->mVertices[i].y;
            vertex.position.z = mesh->mVertices[i].z;
        }

        if(mesh->HasNormals())
        {
            vertex.normals.x = mesh->mNormals[i].x;
            vertex.normals.y = mesh->mNormals[i].y;
            vertex.normals.z = mesh->mNormals[i].z;
        }

        if(mesh->HasTextureCoords(0))
        {
            vertex.uv.x = mesh->mTextureCoords[0][i].x;
            vertex.uv.y = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertex.uv = fm::math::vec2(0,0);
        }
    }

    for(size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for(size_t j = 0; j < face.mNumIndices; ++j)
        {
            meshContainer->listIndices.push_back(face.mIndices[i]);
        }
    }
    inModel->AddMesh(meshContainer);
    //TODO process materials
    //TODO process textures
}

void MeshLoader::ProcessNode(Model *inModel, aiNode *node, const aiScene *scene)
{
    for(size_t i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(inModel, mesh, scene);
    }

    for(size_t i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(inModel, node->mChildren[i], scene);
    }
}

