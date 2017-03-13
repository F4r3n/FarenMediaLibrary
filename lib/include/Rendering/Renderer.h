#pragma once
#include <GL/glew.h>
#include "Rendering/Texture.h"
namespace fm
{
class Renderer
{
public:
    Renderer();
    ~Renderer();
    static inline Renderer& getInstance() {
        return _instance;
    }

    void createQuadScreen();
    //void blur(GLuint *colorBuffer, GLuint *pingpongFBO, GLuint *pingpongColorbuffers);
    void lightComputation(Texture *colorBuffer);
    void postProcess(Texture *colorBuffer);
    void clear();
private:
    static Renderer _instance;
    
    
    GLuint quadVAO;

};
}