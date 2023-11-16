#pragma once
#include "Rendering/Image.h"
#include "Core/Rect.h"
#include "NonCopyable.h"


#define WITH_TEXTURE_ATLAS 0

#if WITH_TEXTURE_ATLAS
#include <array>
namespace fm {
enum SIZE { S_32, S_64, S_128, S_256, S_512, S_1024, LAST_SIZE };
struct TextureAtlas {
    math::Vector2ui size;
    math::Vector2ui pos;
    SIZE s;
    size_t maxSizeRegion;
    unsigned int id;
};

struct TextureDef {
    Recti rectUV;
    int id;
};

class TextureMapper : public fm_system::NonCopyable {
public:
    TextureMapper();
    ~TextureMapper();
    inline static TextureMapper& get() {
        static TextureMapper instance;
        return instance;
    }
    TextureDef registerTexture(Image& image, Recti rect = { 0, 0, -1, -1 });
    TextureDef registerTexture(const std::string& path, Recti rect = { 0, 0, -1, -1 });

private:
    void addAtlas(SIZE s);
    void checkRect(Image& image, Recti& rect);
    TextureDef fillTextureAtlas(Image& image, Recti rect, std::vector<unsigned char>& content);
    SIZE findSize(const Recti& rect);

    TextureAtlas& getCurrent(SIZE s);
    std::array<std::vector<TextureAtlas>, SIZE::LAST_SIZE> maps;
    size_t _maxSize;
};
}
#endif