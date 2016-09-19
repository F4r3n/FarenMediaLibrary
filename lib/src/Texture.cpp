
#include "Texture.h"
#include "ResourcesManager.h"

using namespace fm;
Texture::Texture(const std::string& path, Recti rect, bool alpha)
    : path(path) {
    if(alpha) {
        format = GL_RGBA;
    } else
        format = GL_RGB;

    Image image;
    image.loadImage(path);
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

    glTexImage2D(GL_TEXTURE_2D, 0, format, rect.w, rect.h, 0, format, GL_UNSIGNED_BYTE, content.data());

    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S,
                    GL_REPEAT); // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // stbi_image_free(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
    image.clear();
    content.clear();
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
    std::cout << "Texture " << content.size() << std::endl;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rect.w, rect.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, content.data());

    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S,
                    GL_REPEAT); // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // stbi_image_free(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    content.clear();
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

void Texture::clear() {
    // image.clear();
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, id);
}

Texture::~Texture() {
}
