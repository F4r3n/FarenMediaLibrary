#pragma once
#include <cstddef>
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
        void Generate(size_t size, int indexBuffer);
        void SetData(void *data, size_t size) const;
        void LinkWithShader(Shader *shader, int index, const std::string &name) const;
		unsigned int GetBindingPoint() const { return _bindingPoint; }
        void Free();
        void Bind() const;
    private:
        unsigned int _bindingPoint;
        unsigned int _ubo;
};
}


