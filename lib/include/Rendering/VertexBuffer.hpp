#pragma once
#include <Core/Math/Vector2.h>
#include "Rendering/OpenGL/OGLGraphicsDef.hpp"
#include <Rendering/Mesh.hpp>
namespace fm
{
	namespace rendering
	{
		class VertexBuffer
		{
		public:
			VertexBuffer();
			~VertexBuffer();
			void generate(const std::vector<Vertex> &vertices);
			void generate();
			void setBufferData(void* data, unsigned int size, unsigned int dataSize, bool staticData = true);
			void prepareData();
			void destroy();
			bool isGenerated();
			void GenerateEmpty(size_t maxVertices);
			bool AddVertices(Vertex *inVertices, size_t number, size_t offset);
			size_t GetNumberVertices() const { return _numberVertices; }

		private:
			bool _vaoIsSet;
			size_t _numberVertices;
			unsigned int _indexVBO;
			unsigned int _indexVAO;
		};
	}
}
