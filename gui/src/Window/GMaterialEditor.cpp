#include "GMaterialEditor.h"
#include "Components/CTransform.h"
#include "Core/GameObject.h"
#include <memory>
#include "Components/CIdentity.h"
#include "Components/CCamera.h"
#include "Core/application.h"
#include "Window.h"
using namespace gui;

GMaterialEditor::GMaterialEditor() : GWindow("Material Editor", true)
{
	_kind = gui::WINDOWS::WIN_MATERIAL_EDITOR;

	_go = std::make_unique<fm::GameObject>();

	_go->addComponent<fmc::CTransform>(fm::math::Vector3f(0, 0, 0), fm::math::Vector3f(1, 1, 1), fm::math::vec3(0, 0, 0));
	_go->addComponent<fmc::CIdentity>();
	_go->get<fmc::CIdentity>()->SetNameEntity("GameObject");
	fmc::CCamera* cam = _go->addComponent<fmc::CCamera>(200, 200, fmc::RENDER_MODE::FORWARD, false, false, fm::Application::Get().GetWindow()->GetMSAA());
	cam->Init();
	
}


void GMaterialEditor::_Update(float dt, Context &inContext)
{
	
}


void GMaterialEditor::CustomDraw()
{
	
}


GMaterialEditor::~GMaterialEditor()
{
}
