#include "OGLModel.hpp"
#include "Rendering/Model.hpp"
#include "OGLVertexBuffer.hpp"
#include <GL/glew.h>
using namespace fm;

OGLModel::OGLModel(std::shared_ptr<fm::Model> inModel)
{
	_model = inModel;
	//inModel->SetDestroyCallback(std::bind(&OGLModel::Destroy, this));
}


bool OGLModel::UploadData()
{
	for (auto& mesh : _model->_meshes)
	{
		if (mesh.vertexBuffer == nullptr)
		{
			mesh.vertexBuffer = std::unique_ptr<rendering::VertexBuffer>(new fm::rendering::OGLVertextBuffer());
		}
		mesh.vertexBuffer->UploadData(*mesh.meshContainer);
	}
	return true;
}

bool OGLModel::Destroy()
{
	for (auto& mesh : _model->_meshes)
	{
		if (mesh.vertexBuffer != nullptr)
		{
			mesh.vertexBuffer->destroy();
		}
	}
	_isReady = false;
	return true;
}

void OGLModel::_BindIndex(size_t index) const
{
	if (auto v = dynamic_cast<fm::rendering::OGLVertextBuffer*>(_model->_meshes[index].vertexBuffer.get()))
		v->Bind();
}

void OGLModel::_PrepareBuffer()
{
	for (auto& mesh : _model->_meshes)
	{
		if (auto v = dynamic_cast<fm::rendering::OGLVertextBuffer*>(mesh.vertexBuffer.get()))
		{
			v->prepareData();
		}
	}
	_isReady = true;
}

void OGLModel::Draw()
{
	if (!_isReady)
	{
		_PrepareBuffer();
	}

	for (size_t i = 0; i < _model->GetNumberMeshes(); ++i)
	{
		fm::rendering::MeshContainer* mesh = _model->GetMeshContainer(i);
		_BindIndex(i);
		glDrawElements(GL_TRIANGLES, (GLsizei)mesh->listIndices.size(), GL_UNSIGNED_INT, (void*)(size_t*)mesh->listIndices.data());
		//glDrawElementsInstancedBaseInstance(mode, count, type, indices, 1 /* one instance */, instance);
	}
}

void OGLModel::DrawInstance(uint32_t inNumber, uint32_t inBaseInstance)
{
	if (!_isReady)
	{
		_PrepareBuffer();
	}

	for (size_t i = 0; i < _model->GetNumberMeshes(); ++i)
	{
		fm::rendering::MeshContainer* mesh = _model->GetMeshContainer(i);
		_BindIndex(i);
		glDrawElementsInstancedBaseInstance(GL_TRIANGLES, (GLsizei)mesh->listIndices.size(),
			GL_UNSIGNED_INT, (void*)(size_t*)mesh->listIndices.data(), inNumber, inBaseInstance);
	}
}

