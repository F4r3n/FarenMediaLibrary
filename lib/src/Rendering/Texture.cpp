
#include "Rendering/Texture.h"
#include "Resource/ResourcesManager.h"

using namespace fm;
Texture::Texture(const std::string& path, Recti rect, bool alpha)
    : path(path) {
    if(alpha) {
        format = GL_RGBA;
    } else
        format = GL_RGB;

    Image image;
    if(!image.loadImage(path)) {
        std::cout << "Error loading image " << path << std::endl;
    }
    
    // std::cout << "Texture " << texture.path << " Loaded " << texture.width << " " << texture.height << std::endl;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object

    if(rect.w == -1 && rect.h == -1) {
        rect.w = image.getSize().x - rect.x;
        rect.h = image.getSize().y - rect.y;
    } else if(rect.x > image.getSize().x) {
        rect.x = 0;
    } else if(rect.y > image.getSize().y) {
        rect.y = 0;
    }
    image.getPart(content, rect);
    this->width = rect.w;
    this->height = rect.h;
    std::cout << rect.w << " " << rect.h << std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, format, rect.w, rect.h, 0, format, GL_UNSIGNED_BYTE, content.data());

    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S,
                    GL_REPEAT); 
                    // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //stbi_image_free(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
    image.clear();
    content.clear();
    if(glGetError() != 0)
        std::cout << "Error Loading texture from path "<< path <<  std::endl;
}

Texture::Texture(const Image& image, Recti rect) {
    // std::cout << "Texture " << texture.path << " Loaded " << texture.width << " " << texture.height << std::endl;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object

    if(rect.w == -1 && rect.h == -1) {
        rect.w = image.getSize().x - rect.x;
        rect.h = image.getSize().y - rect.y;
    } else if(rect.x > image.getSize().x) {
        rect.x = 0;
    } else if(rect.y > image.getSize().y) {
        rect.y = 0;
    }
    image.getPart(content, rect);
    this->width = rect.w;
    this->height = rect.h;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rect.w, rect.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, content.data());

    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S,
                    GL_REPEAT); // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //stbi_image_free(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    content.clear();
}

void Texture::generate(int width, int height, Format format, Type type) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    
    GLenum internalFormat;
    if(type == Type::FLOAT) {
        if(format == Format::RGBA)
            internalFormat = GL_RGBA32F;
        else if(format == Format::RGB)
            internalFormat = GL_RGB16F;
        else if(format == Format::RED)
            internalFormat = GL_R32F;
    } else if(type == Type::HALF_FLOAT) {
          if(format == Format::RGBA)
            internalFormat = GL_RGBA16F;
        else if(format == Format::RGB)
            internalFormat = GL_RGB16F;
        else if(format == Format::RED)
            internalFormat = GL_R16F;
    }else if(type == Type::UNSIGNED_BYTE) {
          internalFormat = format;
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    glBindTexture(GL_TEXTURE_2D, 0);
}


Texture::Texture(unsigned int width, unsigned int height) {

    this->width = width;
    this->height = height;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture::setData(unsigned char* image, bool alpha) {
    glBindTexture(GL_TEXTURE_2D, id);
    if(!alpha)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_INT, NULL);
}

Texture::Texture(std::vector<unsigned char>& data, Recti& rect, bool alpha) {
    init(data, rect);
}

void Texture::init(std::vector<unsigned char>& data, Recti& rect) {
    this->width = rect.w;
    this->height = rect.h;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rect.w, rect.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S,
                    GL_REPEAT); // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
}

Texture::Texture() {
}

void Texture::release() {
    //image.clear();
    glDeleteTextures(1, &id);
}

void Texture::clear() {
    // image.clear();
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, id);
}

Texture::~Texture() {
}
