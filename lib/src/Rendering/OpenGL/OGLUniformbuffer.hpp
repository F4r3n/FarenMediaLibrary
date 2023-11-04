#pragma once
#include <string>

namespace fm {
class Shader;
}
namespace fm
{
class OGLUniformbuffer
{
    public:
		OGLUniformbuffer();
        ~OGLUniformbuffer();
        void Generate(size_t size, int indexBuffer, int bufferType);
        void SetData(void *data, size_t size) const;
        void LinkWithShader(Shader *shader, int index, const std::string &name) const;
		unsigned int GetBindingPoint() const { return _bindingPoint; }
        void Free();
        void Bind() const;
		void SetBindingPoint(uint32_t inBindingPoint);
		bool IsValid() const { return _ubo > 0; }
    private:
        unsigned int _bindingPoint = 0;
        unsigned int _ubo = 0;
		uint32_t	 _bufferType = 0;
		size_t		 _size = 0;
};
}


