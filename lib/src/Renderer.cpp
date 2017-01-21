#include "Renderer.h"
#include "ResourcesManager.h"
#include <iostream>

using namespace fm;

Renderer Renderer::_instance;

Renderer::Renderer() {
}



void Renderer::createQuadScreen() {
    GLfloat quadVertices[] = {
        // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // Positions   // TexCoords
        -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f, 1.0f
    };

    GLuint quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glBindVertexArray(0);
}

//void Renderer::blur(GLuint *colorBuffer, GLuint *pingpongFBO, GLuint *pingpongColorbuffers) {
//    GLboolean horizontal = true, first_iteration = true;
//    GLuint amount = 10;
//    std::shared_ptr<Shader> s = ResourcesManager::get().getShader("blur");
//    s->Use();
//    for(GLuint i = 0; i < amount; i++) {
//        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
//        s->setInt("horizontal", horizontal);
//        glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffer[1] : pingpongColorbuffers[!horizontal]);
//        glBindVertexArray(quadVAO);
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//        glBindVertexArray(0);
//        horizontal = !horizontal;
//        if(first_iteration)
//            first_iteration = false;
//    }
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}

void Renderer::lightComputation(GLuint *colorBuffer, GLuint lightBuffer) {

    std::shared_ptr<Shader> light = ResourcesManager::get().getShader("light");
    light->Use();
    glBindFramebuffer(GL_FRAMEBUFFER, lightBuffer);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffer[0]);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, colorBuffer[2]);
    
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Renderer::postProcess(GLuint *colorBuffer, bool horizontal) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffer[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, colorBuffer[1]);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}


void Renderer::clear() {
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Renderer::~Renderer() {
}