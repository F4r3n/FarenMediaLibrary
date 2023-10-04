#include "OGLModel.hpp"
#include "Rendering/Model.hpp"
#include "OGLVertexBuffer.hpp"
using namespace fm;

OGLModel::OGLModel(std::shared_ptr<fm::Model> inModel)
{
	_model = inModel;
	inModel->_destroyCallback = std::bind(&OGLModel::Destroy, this);
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
}

void OGLModel::Draw()
{
	_PrepareBuffer();

	for (size_t i = 0; i < _model->GetNumberMeshes(); ++i)
	{
		fm::rendering::MeshContainer* mesh = _model->GetMeshContainer(i);
		_BindIndex(i);
		glDrawElements(GL_TRIANGLES, (GLsizei)mesh->listIndices.size(), GL_UNSIGNED_INT, (void*)(size_t*)mesh->listIndices.data());
	}
}
