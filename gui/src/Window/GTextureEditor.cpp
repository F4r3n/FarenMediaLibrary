#include "GTextureEditor.hpp"
#include "Rendering/Texture2D.hpp"
#include "Rendering/Texture.h"
#include "Resource/ResourcesManager.h"
using namespace gui;


GTextureEditor::GTextureEditor() : GWindow("TextureEditor", true)
{
	_kind = gui::WINDOWS::WIN_TEXTURE_EDITOR;
}

GTextureEditor::~GTextureEditor()
{

}
void GTextureEditor::CustomDraw()
{
	if (_textureIcon != nullptr)
	{
		ImGui::Image((ImTextureID)(intptr_t)(_textureIcon->GetID()), ImVec2(256, 256));
	}
}

void GTextureEditor::_Update(float dt, Context& inContext)
{
	if (inContext.currentPathSelected.has_value())
	{
		auto texture = fm::ResourcesManager::get().getResource<fm::Texture>(inContext.currentPathSelected.value());
		if (texture != nullptr)
		{
			if (_texture == nullptr || (_texture->GetObjectID() != texture->GetObjectID()))
			{
				_texture = texture;
				_textureIcon = fm::Texture2D::CreateTexture2D(GRAPHIC_API::OPENGL, _texture->GetPath());
			}
		}
	}
}