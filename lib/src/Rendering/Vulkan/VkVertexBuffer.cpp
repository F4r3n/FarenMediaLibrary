#include <Rendering/Vulkan/VkVertexBuffer.hpp>
#include <Core/Config.h>
#include "Rendering/Mesh.hpp"
#include <cstring>

using namespace fm;
using namespace rendering;
VkVertexBuffer::VkVertexBuffer(VmaAllocator inAllocator) : _allocator(inAllocator)
{
	_numberVertices = 0;
}

VkVertexBuffer::~VkVertexBuffer()
{
}

void VkVertexBuffer::destroy()
{
	if(_allocatedBuffer._buffer != nullptr)
		vmaDestroyBuffer(_allocator, _allocatedBuffer._buffer, _allocatedBuffer._allocation);

	_allocatedBuffer._buffer = nullptr;

	if (_allocatedIndexBuffer._buffer != nullptr)
		vmaDestroyBuffer(_allocator, _allocatedIndexBuffer._buffer, _allocatedIndexBuffer._allocation);

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
bool VkVertexBuffer::_SetupBuffer(AllocatedBuffer& buffer, const std::vector<T>& inData, int TYPE_USAGE)
{
	//allocate vertex buffer
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = inData.size() * sizeof(inData[0]);
	bufferInfo.usage = TYPE_USAGE;


	//let the VMA library know that this data should be writeable by CPU, but also readable by GPU
	VmaAllocationCreateInfo vmaallocInfo = {};
	vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

	//allocate the buffer
	vmaCreateBuffer(_allocator, &bufferInfo, &vmaallocInfo,
		&buffer._buffer,
		&buffer._allocation,
		nullptr);

	void* data;
	vmaMapMemory(_allocator, buffer._allocation, &data);
	memcpy(data, inData.data(), bufferInfo.size);
	vmaUnmapMemory(_allocator, buffer._allocation);
	return true;
}



void VkVertexBuffer::UploadData(const fm::rendering::MeshContainer& inMeshContainer)
{
	_numberVertices = inMeshContainer.vertices.size();
	_SetupBuffer(_allocatedBuffer, inMeshContainer.vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
	_SetupBuffer(_allocatedIndexBuffer, inMeshContainer.listIndices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
}