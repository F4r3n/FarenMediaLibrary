#pragma once
#include <memory>
#include "Rendering/Vulkan/Vulkan.hpp"
#include <functional>
namespace fm
{
	class Model;

	class VkVertexBuffer;

	class VkModel
	{
	public:
		VkModel(Vulkan* inVulkan, std::shared_ptr<fm::Model> inModel);
		bool	UploadData(std::function<void(std::function<void(VkCommandBuffer cmd)>)>&& inSubmit);
		bool	Destroy();
		void	Draw(VkCommandBuffer inCmd, uint32_t inInstanceIndex);
	private:
		std::shared_ptr<Model> _model = nullptr;
		std::vector<std::shared_ptr<fm::VkVertexBuffer>> _vertexes;
		Vulkan* _vulkan = nullptr;
	};
}