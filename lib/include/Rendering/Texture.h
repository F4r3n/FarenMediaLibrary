#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Image.h"
#include "Core/Rect.h"
namespace fm {

    enum Format {
        RED = GL_R,
        RGB = GL_RGB,
        RGBA = GL_RGBA,
        DEPTH = GL_DEPTH_COMPONENT,
        STENCIL = GL_STENCIL_INDEX
        
    };
    
    enum Type {
        FLOAT = GL_FLOAT,
        HALF_FLOAT = GL_HALF_FLOAT,
        UNSIGNED_BYTE = GL_UNSIGNED_BYTE
        
    };
    
    enum Filter {
        NEAREST = GL_NEAREST,
        LINEAR = GL_LINEAR
    };
    
    enum Wrapping {
        REPEAT = GL_REPEAT,
        CLAMP_BORDER = GL_CLAMP_TO_BORDER,
        MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
        CLAMP_EDGE = GL_CLAMP_TO_EDGE
    };
     
    
class Texture {

    public:

    Texture(const Image& image, Recti rect = {0,0,-1,-1});
    Texture(const std::string& path, Recti rect = { 0, 0, -1, -1 }, bool alpha = true);
    Texture(std::vector<unsigned char>& data, Recti& rect, bool alpha = true);
    Texture(unsigned int width, unsigned int height);
    void setData(unsigned char* image, bool alpha);
    Texture();
    ~Texture();
    void bind() const;
    void clear();
    inline unsigned int getID() {
        return id;
    }
    void generate(int width, int height, Format format, Type type);
    void release();
    Filter filter;
    Wrapping wrapping = REPEAT;
    private:
    
    
    int width;
    int height;
    void init(std::vector<unsigned char>& data, Recti& rect);
    int format;
    std::string path;
    std::vector<unsigned char> content;

    GLuint id;

};
}

// TODO soil free image