#pragma once
#include <Core/Config.h>

#include "Rendering/Graphics.hpp"

namespace fm
{
	class RenderTexture;
	class OGLTexture;
	class OGLModel;
	class OGLShader;
}

namespace fm
{

	class OGLRenderer
	{
	public:
		OGLRenderer();
		~OGLRenderer();
		static inline OGLRenderer& getInstance() {
			return _instance;
		}

		void SetQuadScreen(fm::OGLModel* inModel);
		// void blur(GLuint *colorBuffer, GLuint *pingpongFBO, GLuint *pingpongColorbuffers);
		void lightComputation(fm::Graphics& graphics, const OGLTexture& incolorBuffer, bool compute = false);
		void postProcess(fm::Graphics& graphics, const OGLTexture& inTexture1);
		void clear(fm::Graphics& graphics);
		void blit(fm::Graphics& graphics, OGLTexture& texture, OGLShader* shader) const;
		void blit(fm::Graphics& graphics, const RenderTexture& source, RenderTexture& dest, BUFFER_BIT bufferBit) const;
		void blit(fm::Graphics& graphics, const RenderTexture& source, BUFFER_BIT bufferBit) const;

		void blit(fm::Graphics& graphics, const RenderTexture& source, RenderTexture& dest, OGLShader* shader) const;
		void blit(fm::Graphics& graphics, int ID, RenderTexture& dest, OGLShader* shader) const;
		void SetSources(fm::Graphics& graphics, const std::vector<OGLTexture>& intextures, size_t numberIDs);
		void blit(fm::Graphics& graphics, RenderTexture& dest, OGLShader* shader);
		void blit(fm::Graphics& graphics, OGLShader* shader);

	private:
		static OGLRenderer _instance;

		fm::OGLModel* _quad;
	};
}