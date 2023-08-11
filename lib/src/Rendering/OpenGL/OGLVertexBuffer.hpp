#pragma once
#include <Core/Math/Vector2.h>
#include "Rendering/OpenGL/OGLGraphicsDef.hpp"
#include <Rendering/Mesh.hpp>
#include "Rendering/VertexBuffer.hpp"
namespace fm
{
	namespace rendering
	{
		class OGLVertextBuffer : public VertexBuffer
		{
		public:
			enum class DATA_TYPE
			{
				FLOAT
			};
			size_t DataTypeToOpengl(DATA_TYPE inType) const;

			OGLVertextBuffer();
			~OGLVertextBuffer();
			void generate(const std::vector<Vertex> &vertices);
			void generate();
			void SetVertexAttribArray(size_t index, size_t size, DATA_TYPE type, size_t stride);
			void setBufferData(void* data, unsigned int offset, unsigned int size, unsigned int dataSize, bool staticData = true);
			void prepareData();
			void Bind() const;
			void destroy();
			bool isGenerated();
			void GenerateEmpty(size_t maxVertices);
			bool AddVertices(Vertex *inVertices, size_t number, size_t offset);
			size_t GetNumberVertices() const { return _numberVertices; }

		private:
			bool _vaoIsSet;
			size_t _numberVertices;
			unsigned int _indexVBO = 0;
			unsigned int _indexVAO = 0;
		};
	}
}
