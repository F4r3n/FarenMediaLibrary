#include "Rendering/commandBuffer.hpp"
#include "Rendering/RenderTexture.hpp"
#include "Rendering/OpenGL/OGLTexture.hpp"

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


void CommandBuffer::Blit(RenderTexture &inSource, RenderTexture &inDestination, std::shared_ptr<fm::Material> inMaterial)
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


void CommandBuffer::Blit(OGLTexture&inSource, RenderTexture &inDestination, std::shared_ptr<fm::Material> inMaterial)
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

void CommandBuffer::DrawMesh(std::shared_ptr<Model> inModel, const Transform &inTranform, std::shared_ptr<fm::Material> inMaterial, const fm::MaterialValues& inMaterialProperties)
{
	Command cmd;
	cmd._command = Command::COMMAND_KIND::DRAW_MESH;
	cmd._model = inModel;
	cmd._material = inMaterial;
	cmd._transform = inTranform;
	cmd._materialProperties = inMaterialProperties;
	_commands.push(cmd);
}

void CommandBuffer::DrawInstancedMesh(const std::vector<std::shared_ptr<Model>>& inModel, const std::vector<Transform>& inTranform,
	std::vector<std::shared_ptr<fm::Material>> inMaterial)
{
	Command cmd;
	cmd._command = Command::COMMAND_KIND::DRAW_INSTACED_MESH;
	for (const auto& model : inModel)
	{
		cmd._models.push_back(model);
	}

	for (const auto& material : inMaterial)
	{
		cmd._materials.push_back(material);
	}

	for (const auto& trans : inTranform)
	{
		cmd._transforms.push_back(trans);
	}

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