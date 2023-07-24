#include "Rendering/commandBuffer.hpp"
#include "Rendering/RenderTexture.h"
#include "Rendering/Texture.h"

using namespace fm;
CommandBuffer::CommandBuffer()
{
}


CommandBuffer::~CommandBuffer()
{

}

void CommandBuffer::Clear(RenderTexture& inSource, BUFFER_BIT inOption)
{
	Command cmd;
	cmd._command = Command::COMMAND_KIND::CLEAR;
	cmd._source.kind = Command::TEXTURE_KIND::RENDER_TEXTURE;
	cmd._source.renderTexture = &inSource;
	cmd._bufferBit = inOption;
	_commands.push(cmd);
}

void CommandBuffer::Clear(BUFFER_BIT inOption)
{
	Command cmd;
	cmd._command = Command::COMMAND_KIND::CLEAR_ALL;
	cmd._bufferBit = inOption;
	_commands.push(cmd);
}


void CommandBuffer::Blit(RenderTexture &inSource, RenderTexture &inDestination, fm::Material *inMaterial)
{
	Command cmd;
	cmd._command = Command::COMMAND_KIND::BLIT;
	cmd._destination.kind = Command::TEXTURE_KIND::RENDER_TEXTURE;
	cmd._destination.renderTexture = &inDestination;
	
	cmd._source.kind = Command::TEXTURE_KIND::RENDER_TEXTURE;
	cmd._source.renderTexture = &inSource;

	cmd._material = inMaterial;
	_commands.push(cmd);

}


void CommandBuffer::Blit(Texture &inSource, RenderTexture &inDestination, fm::Material *inMaterial)
{
	Command cmd;
	cmd._command = Command::COMMAND_KIND::BLIT;
	cmd._destination.kind = Command::TEXTURE_KIND::RENDER_TEXTURE;
	cmd._destination.renderTexture = &inDestination;

	cmd._source.kind = Command::TEXTURE_KIND::TEXTURE;
	cmd._source.texture = &inSource;

	cmd._material = inMaterial;
	_commands.push(cmd);
}

void CommandBuffer::Enable(RENDERING_TYPE inType)
{
	Command cmd;
	cmd._command = Command::COMMAND_KIND::ENABLE;
	cmd._renderingType = inType;
	_commands.push(cmd);

}
void CommandBuffer::Disable( RENDERING_TYPE inType)
{
	Command cmd;
	cmd._command = Command::COMMAND_KIND::DISABLE;
	cmd._renderingType = inType;
	_commands.push(cmd);

}

void CommandBuffer::DrawMesh(Model *inModel, const Transform &inTranform, Material *inMaterial, const fm::MaterialProperties &inMaterialProperties)
{
	Command cmd;
	cmd._command = Command::COMMAND_KIND::DRAW_MESH;
	cmd._model = inModel;
	cmd._material = inMaterial;
	cmd._transform = inTranform;
	cmd._materialProperties = inMaterialProperties;
	_commands.push(cmd);
}


Command CommandBuffer::Pop()
{
	Command cmd = _commands.front();
	_commands.pop();
	return cmd;
}


bool CommandBuffer::IsEmpty() const
{
	return _commands.empty();
}


void CommandBuffer::Push(Command && inCommand)
{
	_commands.push(inCommand);
}