#include "RFont.h"
#include <iostream>
RFont::RFont(const std::string& path) {
    type = fm::RESOURCE_TYPE::FONT;
    unsigned int size = 32;
    FT_Face face;
    FT_Library ft;
    if(FT_Init_FreeType(&ft))
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    // Load font as face

    if(FT_New_Face(ft, path.c_str(), 0, &face))
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(GLubyte c = 0; c < 128; c++) {
        // Load character glyph
        if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     face->glyph->bitmap.width,
                     face->glyph->bitmap.rows,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = { texture,
                                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                                face->glyph->advance.x };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}
RFont::RFont() {
}
RFont::~RFont() {
}