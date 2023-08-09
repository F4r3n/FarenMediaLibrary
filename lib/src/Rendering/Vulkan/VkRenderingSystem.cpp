#include "Rendering/Vulkan/VkRenderingSystem.h"
#include "Rendering/Vulkan/Vulkan.h"
#include "Window.h"
#include "Resource/ResourcesManager.h"
#include "Rendering/Vulkan/VkShader.h"

using namespace fms;

VkRenderingSystem::VkRenderingSystem(fm::Window& inWindow)
{
	_vulkan = std::make_unique<Vulkan>();
	_vulkan->Init(inWindow.getWindow());

	fm::ResourcesManager::get().LoadShaders(GRAPHIC_API::VULKAN);
	fm::ResourcesManager::get().LoadFonts();
	fm::ResourcesManager::get().LoadMaterials();

	fm::VkShader* shader = dynamic_cast<fm::VkShader *>(fm::ResourcesManager::get().getResource<fm::Shader>("default"));
	shader->Make(_vulkan->GetDevice());

}


void VkRenderingSystem::init(EntityManager& manager, EventManager& event)
{

}
void VkRenderingSystem::pre_update(EntityManager& manager)
{

}
void VkRenderingSystem::update(float dt, EntityManager& manager, EventManager& event)
{

}
void VkRenderingSystem::over()
{

}
void VkRenderingSystem::Start()
{

}
void VkRenderingSystem::Stop()
{

}
VkRenderingSystem::~VkRenderingSystem()
{
	_vulkan->DeInit();
}