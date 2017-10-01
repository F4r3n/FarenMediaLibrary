#pragma once
#include <Core/Config.h>
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
    void lightComputation(Texture* colorBuffer, bool compute = false);
    void postProcess(Texture* colorBuffer);
    void clear();
    void blit(Texture& texture, Shader* shader) const;
    void blit(RenderTexture& source, RenderTexture& dest, Shader* shader) const;
    void blit(int ID, RenderTexture& dest, Shader* shader) const;
    void SetSources(Texture *textures, int numberIDs);
    void blit(RenderTexture& dest, Shader* shader);
        void blit(Shader* shader);

private:
    static Renderer _instance;

    GLuint quadVAO;
};
}