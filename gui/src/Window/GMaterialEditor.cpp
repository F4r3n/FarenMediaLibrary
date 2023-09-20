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
