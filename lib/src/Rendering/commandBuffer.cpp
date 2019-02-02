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

void CommandBuffer::Blit(RenderTexture &inSource, RenderTexture &inDestination, fm::Material *inMaterial)
{
	Command cmd;
	cmd._command = Command::COMMAND_KIND::BLIT;
	cmd._destination.kind = Command::TEXTURE_KIND::RENDER_TEXTURE;
	cmd._destination.renderTexture = &inDestination;
	
	cmd._source.kind = Command::TEXTURE_KIND::RENDER_TEXTURE;
	cmd._source.renderTexture = &inSource;

	cmd._material = inMaterial;
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