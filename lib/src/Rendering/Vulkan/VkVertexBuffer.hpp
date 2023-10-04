#pragma once
#include <Core/Math/Vector2.h>
#include "Rendering/Vulkan/Vulkan.hpp"
#include <array>
#include "Rendering/Mesh.hpp"
#include "Rendering/VertexBuffer.hpp"

namespace fm
{
	namespace rendering
	{
		struct MeshContainer;
	}


	class VkVertexBuffer : public rendering::VertexBuffer
	{
	public:

		VkVertexBuffer(Vulkan* inVulkan, std::function<void(std::function<void(VkCommandBuffer cmd)>)> & inSubmit);
		VkVertexBuffer() = default;
		~VkVertexBuffer();

		//TODO refactor
		virtual void UploadData(const fm::rendering::MeshContainer& inMeshContainer, bool inDynamic = false);
		virtual void destroy();
		virtual bool isGenerated() const { return _allocatedBuffer._buffer != nullptr; }
		static VkVertexInputBindingDescription GetBindingDescription();
		static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
	private:
		template <typename T>
		bool		_SetupBufferCPU_GPU(AllocatedBuffer& buffer, const std::vector<T>& data, int TYPE);

		template <typename T>
		bool		_SetupBufferGPU(AllocatedBuffer& buffer, const std::vector<T>& data, int TYPE);

		Vulkan* _vulkan = nullptr;
		std::function<void(std::function<void(VkCommandBuffer cmd)>)> _submitBuffer;

	public:
		AllocatedBuffer	_allocatedBuffer;
		AllocatedBuffer	_allocatedIndexBuffer;

	};
	
}
