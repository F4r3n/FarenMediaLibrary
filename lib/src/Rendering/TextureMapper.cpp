#include "Rendering/TextureMapper.h"
#include <iostream>
using namespace fm;
TextureMapper::TextureMapper() {
    TextureAtlas atlas;

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
}

TextureMapper::~TextureMapper() {
}

void TextureMapper::addAtlas(SIZE s) {
    TextureAtlas atlas;
    atlas.maxSizeRegion = 32 << s;
    atlas.s = s;
    glGenTextures(1, &atlas.id);
    glBindTexture(GL_TEXTURE_2D, atlas.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, maxSize, maxSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    maps[s].push_back(atlas);
}

TextureDef TextureMapper::registerTexture(Image& image, Recti rect) {
    checkRect(image, rect);

    std::vector<unsigned char> content;
    image.getPart(content, rect);
    return fillTextureAtlas(image, rect, content);
}

SIZE TextureMapper::findSize(const Recti& rect) {
    size_t size = 32;
    size_t maxSizeRect = (rect.w > rect.h) ? rect.w : rect.h;
    if(maxSizeRect > (32 << (LAST_SIZE - 1)))
        return LAST_SIZE;

    for(unsigned int i = 0; i < LAST_SIZE; i++) {
        size <<= i;
        if(size - maxSizeRect < 0) {
            continue;
        } else {
            return (SIZE)i;
        }
    }
    return S_1024;
}

void TextureMapper::checkRect(Image& image, Recti& rect) {
    if(rect.w == -1 && rect.h == -1) {
        rect.w = image.getSize().x - rect.x;
        rect.h = image.getSize().y - rect.y;
    } else if(rect.x > image.getSize().x) {
        rect.x = 0;
    } else if(rect.y > image.getSize().y) {
        rect.y = 0;
    }
}

TextureAtlas& TextureMapper::getCurrent(SIZE s) {
    return maps[s].back();
}

TextureDef TextureMapper::fillTextureAtlas(Image& image, Recti rect, std::vector<unsigned char>& content) {
    SIZE s = findSize(rect);
    TextureDef tex;
    tex.id = -1;
    if(s == LAST_SIZE)
        return tex;
    TextureAtlas atlas = getCurrent(s);
    if((atlas.pos.x + atlas.maxSizeRegion) > maxSize) {
        atlas.pos.x = 0;
        atlas.pos.y += atlas.maxSizeRegion;
        if(atlas.pos.y > maxSize) {
            addAtlas(atlas.s);
            return fillTextureAtlas(image, rect, content);
        }
    }
    tex.id = atlas.id;
    tex.rectUV.x = atlas.pos.x/maxSize;
    tex.rectUV.y = atlas.pos.y/maxSize;
    tex.rectUV.w = rect.w/maxSize;
    tex.rectUV.h = rect.h/maxSize;
    glTexSubImage2D(
        GL_TEXTURE_2D, 0, atlas.pos.x, atlas.pos.y, rect.w, rect.h, GL_RGBA, GL_UNSIGNED_BYTE, content.data());
    return tex;
}

TextureDef TextureMapper::registerTexture(const std::string& path, Recti rect) {
    Image image(path);
    if(!image.LoadImage()) {
        std::cerr << "Error loading image " << path << std::endl;
    }
    checkRect(image, rect);

    std::vector<unsigned char> content;
    image.getPart(content, rect);
    return fillTextureAtlas(image, rect, content);
}
