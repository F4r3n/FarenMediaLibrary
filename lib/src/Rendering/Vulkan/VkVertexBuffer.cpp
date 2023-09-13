#include <Rendering/Vulkan/VkVertexBuffer.hpp>
#include <Core/Config.h>
#include "Rendering/Mesh.hpp"
#include <cstring>

using namespace fm;
using namespace rendering;
VkVertexBuffer::VkVertexBuffer(Vulkan* inVulkan, std::function<void(std::function<void(VkCommandBuffer cmd)>)>& inSubmit)
	: _vulkan(inVulkan),
	_submitBuffer(inSubmit)
{
	_numberVertices = 0;
}

VkVertexBuffer::~VkVertexBuffer()
{
}

void VkVertexBuffer::destroy()
{
	if (_allocatedBuffer._buffer != nullptr)
		_vulkan->DestroyBuffer(_allocatedBuffer);

	_allocatedBuffer._buffer = nullptr;

	if (_allocatedIndexBuffer._buffer != nullptr)
		_vulkan->DestroyBuffer(_allocatedIndexBuffer);

	_allocatedIndexBuffer._buffer = nullptr;
}

VkVertexInputBindingDescription VkVertexBuffer::GetBindingDescription()
{
	VkVertexInputBindingDescription bindingDescription{};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(Vertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return bindingDescription;
}


std::array<VkVertexInputAttributeDescription, 3> VkVertexBuffer::GetAttributeDescriptions()
{
	std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Vertex, position);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, normal);

	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(Vertex, color);

	return attributeDescriptions;
}

template <typename T>
bool VkVertexBuffer::_SetupBufferCPU_GPU(AllocatedBuffer& buffer, const std::vector<T>& inData, int TYPE_USAGE)
{
	const size_t bufferSize = inData.size() * sizeof(inData[0]);

	buffer = _vulkan->CreateBuffer(bufferSize, TYPE_USAGE, VMA_MEMORY_USAGE_CPU_TO_GPU);
	_vulkan->MapBuffer(buffer, (void*)inData.data(), bufferSize, 0);


	return true;
}

template <typename T>
bool VkVertexBuffer::_SetupBufferGPU(AllocatedBuffer& buffer, const std::vector<T>& inData, int TYPE)
{
	const size_t bufferSize = inData.size() * sizeof(inData[0]);
	

	AllocatedBuffer stagingBuffer = _vulkan->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY);
	_vulkan->MapBuffer(stagingBuffer, (void*)inData.data(), bufferSize, 0);


	buffer = _vulkan->CreateBuffer(bufferSize, TYPE | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY);


	_submitBuffer([=](VkCommandBuffer cmd) {
		VkBufferCopy copy;
		copy.dstOffset = 0;
		copy.srcOffset = 0;
		copy.size = bufferSize;
		vkCmdCopyBuffer(cmd, stagingBuffer._buffer, buffer._buffer, 1, &copy);
		});

	_vulkan->DestroyBuffer(stagingBuffer);

	return true;
}



void VkVertexBuffer::UploadData(const fm::rendering::MeshContainer& inMeshContainer, bool inDynamic)
{
	_numberVertices = inMeshContainer.vertices.size();
	if (inDynamic)
	{
		_SetupBufferCPU_GPU(_allocatedBuffer, inMeshContainer.vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
		_SetupBufferCPU_GPU(_allocatedIndexBuffer, inMeshContainer.listIndices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
	}
	else
	{
		_SetupBufferGPU(_allocatedBuffer, inMeshContainer.vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
		_SetupBufferGPU(_allocatedIndexBuffer, inMeshContainer.listIndices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
	}
}