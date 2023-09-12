#include "VkModel.h"
#include "Rendering/Model.hpp"
#include "Rendering/Vulkan/VkVertexBuffer.hpp"
#include "Rendering/VertexBuffer.hpp"
using namespace fm;

VkModel::VkModel(VmaAllocator inAllocator, std::shared_ptr<fm::Model> inModel)
{
	_model = inModel;
	_allocator = inAllocator;
	inModel->_destroyCallback = std::bind(&VkModel::Destroy, this);
}


bool VkModel::UploadData(std::function<void(std::function<void(VkCommandBuffer cmd)>)>&& inSubmit)
{
	for (auto& mesh : _model->_meshes)
	{
		if (mesh.vertexBuffer == nullptr)
		{
			mesh.vertexBuffer = std::unique_ptr<rendering::VertexBuffer>(new fm::VkVertexBuffer(_allocator, std::move(inSubmit)));
		}
		mesh.vertexBuffer->UploadData(*mesh.meshContainer);
	}
	return true;
}

bool VkModel::Destroy()
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

void VkModel::Draw(VkCommandBuffer inCmd, uint32_t inInstanceIndex)
{
	for (auto& mesh : _model->_meshes)
	{
		fm::VkVertexBuffer* vertex = dynamic_cast<fm::VkVertexBuffer*>(mesh.vertexBuffer.get());
		if (vertex == nullptr)
			break;
		VkDeviceSize offset = 0;
		vkCmdBindVertexBuffers(inCmd, 0, 1, &vertex->_allocatedBuffer._buffer, &offset);
		vkCmdBindIndexBuffer(inCmd, vertex->_allocatedIndexBuffer._buffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdDraw(inCmd, vertex->GetNumberVertices(), 1, 0, inInstanceIndex);
	}

}
