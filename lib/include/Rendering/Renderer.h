#pragma once
#include <Core/Config.h>

#include "Rendering/Graphics.hpp"

namespace fm
{
	class RenderTexture;
	class Shader;
	class Texture;
}

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
    void lightComputation(fm::Graphics &graphics, const Texture& incolorBuffer, bool compute = false);
    void postProcess(fm::Graphics &graphics, const Texture& inTexture1);
    void clear(fm::Graphics &graphics);
    void blit(fm::Graphics &graphics,Texture& texture, Shader* shader) const;
	void blit(fm::Graphics &graphics, RenderTexture& source, RenderTexture& dest, BUFFER_BIT bufferBit) const;
	void blit(fm::Graphics &graphics, RenderTexture& source, BUFFER_BIT bufferBit) const;

    void blit(fm::Graphics &graphics,RenderTexture& source, RenderTexture& dest, Shader* shader) const;
    void blit(fm::Graphics &graphics,int ID, RenderTexture& dest, Shader* shader) const;
    void SetSources(fm::Graphics &graphics, const std::vector<Texture> &intextures, int numberIDs);
    void blit(fm::Graphics &graphics,RenderTexture& dest, Shader* shader);
    void blit(fm::Graphics &graphics,Shader* shader);

private:
    static Renderer _instance;

    fm::Model* quad;
};
}