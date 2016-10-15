#pragma once
#include <GL/glew.h>
#include <iostream>
namespace fm
{
class Renderer
{
public:
    Renderer();
    ~Renderer();
    static Renderer& getInstance();
    void initFrameBuffer(unsigned int width, unsigned int height);
    void bindFrameBuffer();
    void createQuadScreen();
    void blur();
    void lightComputation();
    void postProcess(bool horizontal);
    void clear();
private:
    static Renderer _instance;
    
    
    GLuint framebuffer;
    GLuint quadVAO;
    GLuint textureColorbuffer[3];

    GLuint pingpongFBO[2];
    GLuint pingpongColorbuffers[2];
    
    GLuint lightShadowFBO;
    GLuint lightShadowBuffer[2];
    
};
}