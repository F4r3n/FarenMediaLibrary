#pragma once
#include "Image.h"
#include "Vector2.h"
#include "NonCopyable.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Rect.h"
#include <array>
namespace fm {
    enum SIZE {
  S_32,
  S_64,
  S_128,
  S_256,
  S_512,
  S_1024,
  LAST_SIZE
};
struct TextureAtlas {
    Vector2ui size;
    Vector2ui pos;
    SIZE s;
    size_t maxSizeRegion;
    unsigned int id;
};



class TextureMapper : public fm_system::NonCopyable {
public:
    TextureMapper();
    ~TextureMapper();
    inline static TextureMapper& get() {
        static TextureMapper instance;
        return instance;
    }
    unsigned int registerTexture(Image& image, Recti rect = { 0, 0, -1, -1 });
    unsigned int registerTexture(const std::string& path, Recti rect = { 0, 0, -1, -1 });

private:
    void addAtlas(SIZE s);
    void checkRect(Image &image, Recti &rect);
    unsigned int fillTextureAtlas(Image &image, Recti rect, std::vector<unsigned char> &content);
    SIZE findSize(const Recti &rect);
    
    TextureAtlas& getCurrent(SIZE s);
    std::array< std::vector<TextureAtlas>, SIZE::LAST_SIZE> maps;
    int maxSize;
};
}