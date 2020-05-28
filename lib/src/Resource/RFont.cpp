#include <ft2build.h>
#include FT_FREETYPE_H

#include "Resource/RFont.h"
#include <iostream>
#include <algorithm>

#include "Rendering/Texture.h"

#define MAXWIDTH 512
using namespace fm;
RFont::RFont(const std::string& path) : Resource(fm::FilePath(path)) {
    unsigned int size = 32;
    FT_Face face;
    FT_Library ft;
    if(FT_Init_FreeType(&ft))
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library"
                  << std::endl;

    // Load font as face

    if(FT_New_Face(ft, path.c_str(), 0, &face))
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, size);
    FT_GlyphSlot g = face->glyph;
    unsigned int roww = 0;
    unsigned int rowh = 0;
    unsigned int w = 0;
    unsigned int h = 0;
    atlas_width = 0;
    atlas_height = 0;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(int i = 32; i < 128; i++) {
        if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            fprintf(stderr, "Loading character %c failed!\n", i);
            continue;
        }
        if(roww + g->bitmap.width + 1 >= MAXWIDTH) {
            w = std::max(w, roww);
            h += rowh;
            roww = 0;
            rowh = 0;
        }
        roww += g->bitmap.width + 1;
        rowh = std::max(rowh, g->bitmap.rows);
    }
    w = std::max(w, roww);
    h += rowh;
    atlas_width = w;
    atlas_height = h;

    texture = new fm::Texture();
    texture->wrapping = fm::Wrapping::CLAMP_EDGE;
    texture->filter = fm::Filter::LINEAR;
    texture->generate(w, h, fm::Format::RED, fm::Type::UNSIGNED_BYTE);
    texture->bind();

    int ox = 0;
    int oy = 0;
    int numberCanals = 1;

    rowh = 0;
    for(int i = 32; i < 128; i++) {
        if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            fprintf(stderr, "Loading character %c failed!\n", i);
            continue;
        }
        unsigned char* tempBuffer =
            new unsigned char[g->bitmap.width * g->bitmap.rows * numberCanals];
        for(unsigned int j = 0; j < g->bitmap.width * g->bitmap.rows; ++j) {
            tempBuffer[j * numberCanals + 0] = g->bitmap.buffer[j];
            //tempBuffer[j * 4 + 1] = g->bitmap.buffer[j];
            //tempBuffer[j * 4 + 2] = g->bitmap.buffer[j];
            //tempBuffer[j * 4 + 3] = 255;

        }

        if(ox + g->bitmap.width + 1 >= MAXWIDTH) {
            oy += rowh;
            rowh = 0;
            ox = 0;
        }

        texture->setData(tempBuffer, fm::Recti(ox, oy, g->bitmap.width, g->bitmap.rows));

        //#endif
        Character character = {
            fm::math::vec2(g->advance.x >> 6, g->advance.y >> 6),
            fm::math::vec2(g->bitmap.width, g->bitmap.rows),
            fm::math::vec2(g->bitmap_left, g->bitmap_top),
            fm::math::vec2(ox / (float)w, oy / (float)h)};
        Characters.insert(std::pair<GLchar, Character>(i, character));
        rowh = std::max(rowh, g->bitmap.rows);
        ox += g->bitmap.width + 1;
        delete tempBuffer;
    }
	//texture->writeToPNG("C:/Users/guill/Downloads/test.png");

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    std::cout << "Font loaded " << !glGetError() << std::endl;
    // exit(-1);
}
RFont::RFont() : Resource(fm::FilePath("")) {
}
RFont::~RFont() {
}
