#pragma once
#include <queue>
#include "Rendering/RenderQueue.h"
#include "Rendering/MaterialValue.h"
#include "Rendering/material.hpp"
#include "Core/Transform.h"


namespace fm
{
	class CommandBuffer;
	class OGLTexture;
	class RenderTexture;
	class Model;

	struct Command
	{
		enum COMMAND_KIND
		{
			CLEAR,
			CLEAR_ALL,
			BLIT,
			DRAW_MESH,
			DRAW_INSTACED_MESH,
			ENABLE,
			DISABLE
		};

		enum TEXTURE_KIND
		{
			TEXTURE,
			RENDER_TEXTURE
		};

		struct TextureKind
		{
			TEXTURE_KIND kind;
			OGLTexture*texture = nullptr;
			RenderTexture *renderTexture = nullptr;
		};
	public:
		COMMAND_KIND _command;
		fm::MaterialValue _materialValue;
		TextureKind _source;
		TextureKind _destination;
		std::weak_ptr<Material> _material;
		std::weak_ptr<Model> _model;
		Transform _transform;
		MaterialProperties _materialProperties;
		BUFFER_BIT _bufferBit;
		RENDERING_TYPE _renderingType;

		std::vector< std::weak_ptr<Material>> _materials;
		std::vector< std::weak_ptr<Model>> _models;
		std::vector<Transform> _transforms;
	};

	class CommandBuffer
	{
	public:
		CommandBuffer();
		~CommandBuffer();

		void Clear(RenderTexture& inSource, BUFFER_BIT inOption);
		void Clear(BUFFER_BIT inOption);

		void Enable(RENDERING_TYPE inType);
		void Disable(RENDERING_TYPE inType);

		void Blit(RenderTexture &inSource, RenderTexture &inDestination, std::shared_ptr<fm::Material> inMaterial = nullptr);
		void Blit(OGLTexture&inSource, RenderTexture &inDestination, std::shared_ptr<fm::Material> inMaterial = nullptr);
		void DrawMesh(std::shared_ptr<Model> inModel, const Transform &inTranform, std::shared_ptr<fm::Material> inMaterial, const fm::MaterialProperties &inMaterialProperties);
		void DrawInstancedMesh(const std::vector<std::shared_ptr<Model>>& inModel, const std::vector<Transform>& inTranform,
			std::vector<std::shared_ptr<fm::Material>> inMaterial);

		Command Pop();
		bool IsEmpty() const;
		void Push(Command && inCommand);
	private:
		std::queue<Command> _commands;
	};
}