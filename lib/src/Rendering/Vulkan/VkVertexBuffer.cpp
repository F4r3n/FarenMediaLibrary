#include <Rendering/Vulkan/VkVertexBuffer.hpp>
#include <Core/Config.h>
#include "Rendering/Mesh.hpp"
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

bool VkVertexBuffer::_SetupVertexBuffer(const std::vector<rendering::Vertex>& vertices)
{
	_numberVertices = vertices.size();
	//allocate vertex buffer
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	//this is the total size, in bytes, of the buffer we are allocating
	bufferInfo.size = vertices.size() * sizeof(Vertex);
	//this buffer is going to be used as a Vertex Buffer
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;


	//let the VMA library know that this data should be writeable by CPU, but also readable by GPU
	VmaAllocationCreateInfo vmaallocInfo = {};
	vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

	//allocate the buffer
	vmaCreateBuffer(_allocator, &bufferInfo, &vmaallocInfo,
		&_allocatedBuffer._buffer,
		&_allocatedBuffer._allocation,
		nullptr);

	void* data;
	vmaMapMemory(_allocator, _allocatedBuffer._allocation, &data);
	memcpy(data, vertices.data(), vertices.size() * sizeof(Vertex));
	vmaUnmapMemory(_allocator, _allocatedBuffer._allocation);
	return true;
}


void VkVertexBuffer::UploadData(const fm::rendering::MeshContainer& inMeshContainer)
{
	_SetupVertexBuffer(inMeshContainer.vertices);
}