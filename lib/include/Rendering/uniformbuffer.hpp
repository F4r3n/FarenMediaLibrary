#ifndef UNIFORMBUFFER_HPP
#define UNIFORMBUFFER_HPP
#include <cstddef>
#include <string>
namespace fm {
class Shader;
}
namespace fm
{
class UniformBuffer
{
    public:
        UniformBuffer();
        ~UniformBuffer();
        void Generate(size_t size, int indexBuffer);
        void SetData(void *data, size_t size);
        void LinkWithShader(Shader *shader, int index, const std::string &name);
        void Free();
        void Bind();
    private:
        unsigned int _bindingPoint;
        unsigned int _ubo;
};
}

#endif // UNIFORMBUFFER_HPP

