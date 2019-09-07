#include "PickingSystem.h"
#include "Core/SceneManager.h"
#include "Core/GameObject.h"
#include "Components/CTransform.h"
#include "Components/CCamera.h"
#include "Window.h"
using namespace fms;

PickingSystem::PickingSystem()
{
	_specialCamera = fm::GameObjectHelper::create(fm::SceneManager::get().getScene("_editor"));
	_specialCamera->addComponent<fmc::CCamera>(fm::Window::kWidth, fm::Window::kHeight, fmc::RENDER_MODE::FORWARD, false, false, 4);

	_specialCamera->addComponent<fmc::CTransform>();
	_specialCamera->name = "Camera";
}


fm::GameObject* PickingSystem::PickGameObject(fm::GameObject* inCurrentCamera, fm::math::vec2 &inPos)
{
	return nullptr;
}
