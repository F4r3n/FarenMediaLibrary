#include "CText.h"
#include <iostream>
using namespace fmc;

CText::CText(const std::string& text, const std::string& fontName, unsigned int width, unsigned int height) {
    this->text = text;
    this->fontName = fontName;

    projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
}

CText::~CText() {
}