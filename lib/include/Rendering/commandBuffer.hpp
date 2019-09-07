#pragma once
#include <queue>
#include "Rendering/RenderQueue.h"
#include "Rendering/MaterialValue.h"
#include "Rendering/material.hpp"
#include "Core/Transform.h"
namespace fms
{
	class RenderingSystem;
}


namespace fm
{
	class CommandBuffer;
	class Texture;
	class RenderTexture;
	class Model;

	struct Command
	{
		enum COMMAND_KIND
		{
			BLIT,
			DRAW_MESH
		};

		enum TEXTURE_KIND
		{
			TEXTURE,
			RENDER_TEXTURE
		};

		struct TextureKind
		{
			TEXTURE_KIND kind;
			Texture *texture = nullptr;
			RenderTexture *renderTexture = nullptr;
		};
	private:
		COMMAND_KIND _command;
		fm::MaterialValue _materialValue;
		TextureKind _source;
		TextureKind _destination;
		Material* _material;
		Model *_model;
		Transform _transform;

		friend class CommandBuffer;
		friend class fms::RenderingSystem;
	};

	class CommandBuffer
	{
	public:
		CommandBuffer();
		~CommandBuffer();

		void Blit(RenderTexture &inSource, RenderTexture &inDestination, fm::Material *inMaterial = nullptr);
		void Blit(Texture &inSource, RenderTexture &inDestination, fm::Material *inMaterial = nullptr);
		void DrawMesh(Model *inModel, const Transform &inTranform, Material *inMaterial);
		Command Pop();
		bool IsEmpty() const;
		void Push(Command && inCommand);
	private:


		std::queue<Command> _commands;

	};
}