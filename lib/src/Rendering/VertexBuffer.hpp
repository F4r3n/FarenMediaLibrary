#pragma once
#if defined(_MSC_VER) && _MSC_VER >= 1900
#pragma warning(disable: 4100)
#endif

#include <Rendering/Mesh.hpp>
namespace fm
{
	namespace rendering
	{
		class VertexBuffer
		{
		public:

			VertexBuffer();
			virtual ~VertexBuffer();
			virtual bool AddVertices([[maybe_unused]] Vertex* inVertices, [[maybe_unused]] size_t number, [[maybe_unused]] size_t offset) { return false; }
			virtual void destroy() { ; }
			virtual bool isGenerated() { return false; }
			size_t GetNumberVertices() const { return _numberVertices; }
			virtual void UploadData(const MeshContainer& inMeshContainer, bool inDynamic = false) = 0;

		protected:
			size_t _numberVertices;
		};
	}
}
