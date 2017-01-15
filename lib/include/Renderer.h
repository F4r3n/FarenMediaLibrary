#pragma once
#include <GL/glew.h>
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
    void blur(GLuint *colorBuffer, GLuint *pingpongFBO, GLuint *pingpongColorbuffers);
    void lightComputation(GLuint *colorBuffer, GLuint lightBuffer);
    void postProcess(GLuint *colorBuffer, bool horizontal);
    void clear();
private:
    static Renderer _instance;
    
    
    GLuint quadVAO;

};
}