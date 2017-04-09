#pragma once
#include <GL/glew.h>
#include "Rendering/Texture.h"
#include <memory>
#include "Rendering/Shader.h"
#include "Rendering/RenderTexture.h"
namespace fm {
class Renderer {
public:
    Renderer();
    ~Renderer();
    static inline Renderer& getInstance() {
        return _instance;
    }

    void createQuadScreen();
    // void blur(GLuint *colorBuffer, GLuint *pingpongFBO, GLuint *pingpongColorbuffers);
    void lightComputation(Texture* colorBuffer);
    void postProcess(Texture* colorBuffer);
    void clear();
    void blit(Texture& texture, std::shared_ptr<Shader> shader) const;
    void blit(RenderTexture& source, RenderTexture& dest, std::shared_ptr<Shader> shader) const;
    void blit(int ID, RenderTexture& dest, std::shared_ptr<Shader> shader) const;
    void SetSources(Texture *textures, int numberIDs);
    void blit(RenderTexture& dest, std::shared_ptr<Shader> shader);
        void blit(std::shared_ptr<Shader> shader);

private:
    static Renderer _instance;

    GLuint quadVAO;
};
}