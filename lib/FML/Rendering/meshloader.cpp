#include "Rendering/meshloader.hpp"
#include <Rendering/Model.hpp>
#include <Rendering/Mesh.hpp>
#include <Core/Debug.h>

#if WITH_ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#endif

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
// Optional. define TINYOBJLOADER_USE_MAPBOX_EARCUT gives robust trinagulation. Requires C++11
#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "tiny_obj_loader.h"

using namespace fm;
using namespace rendering;

std::optional<std::shared_ptr<fm::Model>> MeshLoader::Load(const fm::FilePath& inFilePath, const std::string &inObjectName)
{
#if WITH_ASSIMP
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(inFilePath.GetPath().string(), aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        DEBUG_ERROR("Error import object");
        return std::nullopt;
    }

	std::shared_ptr<fm::Model> model = std::make_shared<Model>(inObjectName);

    ProcessNode(model.get(), scene->mRootNode, scene);
#endif

	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = inFilePath.GetParent().GetPathString();

	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile(inFilePath.GetPathString(), reader_config)) {
		if (!reader.Error().empty()) {
			DEBUG_ERROR("Error import object");
		}
		return std::nullopt;
	}

	std::shared_ptr<fm::Model> model = std::make_shared<Model>(inObjectName);
	ProcessMesh(model.get(), reader);

    return model;
}


void MeshLoader::ProcessMesh(Model* inModel, const tinyobj::ObjReader& inReader)
{
	auto& attrib = inReader.GetAttrib();
	auto& shapes = inReader.GetShapes();
	//auto& materials = inReader.GetMaterials();

	// Loop over shapes

	for (size_t s = 0; s < shapes.size(); s++) {
		fm::rendering::MeshContainer* meshContainer = new fm::rendering::MeshContainer();
		meshContainer->vertices.reserve(shapes[s].mesh.num_face_vertices.size());
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {

			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			Vertex vertex;
			vertex.color = fm::math::vec3(1, 1, 1);

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				vertex.position.x = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				vertex.position.y = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
				vertex.position.z = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
				
				if (idx.normal_index >= 0) {
					vertex.normal.x = attrib.normals[3 * size_t(idx.normal_index) + 0];
					vertex.normal.y = attrib.normals[3 * size_t(idx.normal_index) + 1];
					vertex.normal.z = attrib.normals[3 * size_t(idx.normal_index) + 2];
				}

				if (idx.texcoord_index >= 0) {
					vertex.uv.x = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					vertex.uv.y = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
				}
				meshContainer->vertices.push_back(vertex);
				meshContainer->listIndices.push_back(static_cast<uint32_t>(meshContainer->listIndices.size()));

				// Optional: vertex colors
				// tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
				// tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
				// tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
			}
			index_offset += fv;

			// per-face material
			//shapes[s].mesh.material_ids[f];
		}
		inModel->AddMesh(meshContainer);

	}
}

#if WITH_ASSIMP

void MeshLoader::ProcessMesh(Model *inModel, aiMesh *mesh, const aiScene *scene)
{
    fm::rendering::MeshContainer* meshContainer = new fm::rendering::MeshContainer();
	meshContainer->vertices.reserve(mesh->mNumVertices);
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
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
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
		meshContainer->vertices.push_back(vertex);
    }

    for(size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for(size_t j = 0; j < face.mNumIndices; ++j)
        {
            meshContainer->listIndices.push_back(face.mIndices[j]);
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

#endif