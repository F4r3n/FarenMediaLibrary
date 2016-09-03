#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Shader.h" 
#include <map>
#include "Drawable.h"
#include "Vector2.h"
namespace fm {

struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    long Advance;    // Horizontal offset to advance to next glyph
};

class Text : public Drawable{
public:
	Text(const std::string &path, int width, int height);
	~Text();
	void setFontSize(unsigned short size);


	void draw(Shader &shader);
	const std::string getNameShader() const;
private:
	std::map<GLchar, Character> Characters;
	GLuint VAO, VBO;

	FT_Face face;
	FT_Library ft;

	std::string nameShader = "text";

	std::string text = "Ab";

	Vector2f pos = {300,300};
	GLfloat scale = 1;
	glm::vec3 color = glm::vec3(1);
	glm::mat4 projection;
};
}