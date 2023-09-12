#include <Rendering/Vulkan/VkVertexBuffer.hpp>
#include <Core/Config.h>
#include "Rendering/Mesh.hpp"
#include <cstring>

using namespace fm;
using namespace rendering;
VkVertexBuffer::VkVertexBuffer(VmaAllocator inAllocator, std::function<void(std::function<void(VkCommandBuffer cmd)>)>&& inSubmit) : _allocator(inAllocator)
{
	_numberVertices = 0;
	_submitBuffer = inSubmit;
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
bool VkVertexBuffer::_SetupBufferCPU_GPU(AllocatedBuffer& buffer, const std::vector<T>& inData, int TYPE_USAGE)
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

template <typename T>
bool VkVertexBuffer::_SetupBufferGPU(AllocatedBuffer& buffer, const std::vector<T>& inData, int TYPE)
{
	const size_t bufferSize = inData.size() * sizeof(inData[0]);
	//allocate staging buffer
	VkBufferCreateInfo stagingBufferInfo = {};
	stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingBufferInfo.pNext = nullptr;

	stagingBufferInfo.size = bufferSize;
	stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

	//let the VMA library know that this data should be on CPU RAM
	VmaAllocationCreateInfo vmaallocInfo = {};
	vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

	AllocatedBuffer stagingBuffer;

	//allocate the buffer
	vmaCreateBuffer(_allocator, &stagingBufferInfo, &vmaallocInfo,
		&stagingBuffer._buffer,
		&stagingBuffer._allocation,
		nullptr);

	void* data;
	vmaMapMemory(_allocator, stagingBuffer._allocation, &data);
	memcpy(data, inData.data(), bufferSize);
	vmaUnmapMemory(_allocator, stagingBuffer._allocation);

	//allocate vertex buffer
	VkBufferCreateInfo vertexBufferInfo = {};
	vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	vertexBufferInfo.pNext = nullptr;
	vertexBufferInfo.size = bufferSize;
	vertexBufferInfo.usage = TYPE | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

	//let the VMA library know that this data should be GPU native
	vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

	//allocate the buffer
	vmaCreateBuffer(_allocator, &vertexBufferInfo, &vmaallocInfo,
		&buffer._buffer,
		&buffer._allocation,
		nullptr);

	_submitBuffer([=](VkCommandBuffer cmd) {
		VkBufferCopy copy;
		copy.dstOffset = 0;
		copy.srcOffset = 0;
		copy.size = bufferSize;
		vkCmdCopyBuffer(cmd, stagingBuffer._buffer, buffer._buffer, 1, &copy);
		});

	vmaDestroyBuffer(_allocator, stagingBuffer._buffer, stagingBuffer._allocation);

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