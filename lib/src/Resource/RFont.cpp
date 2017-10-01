#include <ft2build.h>
#include FT_FREETYPE_H

#include "Resource/RFont.h"
#include <iostream>
#include <algorithm>
#define MAXWIDTH 2048
RFont::RFont(const std::string& path) : Resource(){
    unsigned int size = 32;
    FT_Face face;
    FT_Library ft;
    if(FT_Init_FreeType(&ft))
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

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

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    #if OPENGL_ES_VERSION > 2
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
    #else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

#endif
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    /* Linear filtering usually looks best for text */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int ox = 0;
    int oy = 0;
    rowh = 0;
    for(int i = 32; i < 128; i++) {
        if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            fprintf(stderr, "Loading character %c failed!\n", i);
            continue;
        }
        if(ox + g->bitmap.width + 1 >= MAXWIDTH) {
            oy += rowh;
            rowh = 0;
            ox = 0;
        }
 #if OPENGL_ES_VERSION > 2
        glTexSubImage2D(
            GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
            #else
            glTexSubImage2D(
            GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_RGBA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
            
#endif
        Character character = { fm::math::vec2(g->advance.x >> 6, g->advance.y >> 6),
                                fm::math::vec2(g->bitmap.width, g->bitmap.rows),
                                fm::math::vec2(g->bitmap_left, g->bitmap_top),
                                fm::math::vec2(ox/(float)w, oy/(float)h)};
        Characters.insert(std::pair<GLchar, Character>(i, character));
        rowh = std::max(rowh, g->bitmap.rows);
        ox += g->bitmap.width + 1;
    }
    
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    std::cout << "Font loaded " << glGetError() << std::endl;
    //exit(-1);
}
RFont::RFont() : Resource(){
}
RFont::~RFont() {
}