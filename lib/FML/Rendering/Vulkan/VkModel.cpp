#include "VkModel.hpp"
#include "Rendering/Model.hpp"
#include "Rendering/Vulkan/VkVertexBuffer.hpp"
#include "Rendering/VertexBuffer.hpp"
using namespace fm;

VkModel::VkModel(Vulkan* inVulkan, std::shared_ptr<fm::Model> inModel)
{
	_model = inModel;
	_vulkan = inVulkan;
	//inModel->SetDestroyCallback(std::bind(&VkModel::Destroy, this));
}


bool VkModel::UploadData(std::function<void(std::function<void(VkCommandBuffer cmd)>)>&& inSubmit)
{
	for (auto& mesh : _model->_meshes)
	{
		auto it = _vertexes.emplace_back(std::make_shared<fm::VkVertexBuffer>(_vulkan, inSubmit));
		it->UploadData(*mesh.meshContainer);
	}
	return true;
}

bool VkModel::Destroy()
{
	for (auto& mesh : _vertexes)
	{
		mesh->destroy();
	}
	return true;
}

void VkModel::Draw(VkCommandBuffer inCmd, uint32_t inInstanceIndex)
{
	for (auto& mesh : _vertexes)
	{
		VkDeviceSize offset = 0;
		vkCmdBindVertexBuffers(inCmd, 0, 1, &mesh->_allocatedBuffer._buffer, &offset);
		vkCmdBindIndexBuffer(inCmd, mesh->_allocatedIndexBuffer._buffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdDraw(inCmd, static_cast<uint32_t>(mesh->GetNumberVertices()), 1, 0, inInstanceIndex);
	}

}
