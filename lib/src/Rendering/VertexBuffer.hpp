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
			virtual bool AddVertices(Vertex* inVertices, size_t number, size_t offset) { return false; }
			virtual void destroy() { ; }
			virtual bool isGenerated() { return false; }
			size_t GetNumberVertices() const { return _numberVertices; }
			virtual void UploadData(const MeshContainer& inMeshContainer) { ; }

		protected:
			size_t _numberVertices;
		};
	}
}
