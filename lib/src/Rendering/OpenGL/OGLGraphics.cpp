#include <Rendering/Graphics.hpp>
#include <Core/Config.h>
#include <Rendering/OpenGL/OGLGraphicsDef.hpp>
#include "Rendering/Model.hpp"
#include "Rendering/VertexBuffer.hpp"
using namespace fm;
using namespace rendering;
Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

void Graphics::Clear(BUFFER_BIT inOption) const
{
    glClear(inOption);
}

void Graphics::SetViewPort(const fm::math::vec4i& rect) const
{
    glViewport(rect.x, rect.y, rect.z, rect.w);
}

void Graphics::SetViewPort(const fm::Rect<int>& rect) const
{
    glViewport(rect.x, rect.y, rect.w, rect.h);
}

void Graphics::Enable(RENDERING_TYPE r) const
{
	auto it = _renderingSettings.find(r);
	if (it != _renderingSettings.end())
	{
		if (!it->second)
		{
			glEnable(r);
		}
		it->second = true;
	}
	else
	{
		glEnable(r);
		_renderingSettings.insert(std::pair<fm::RENDERING_TYPE, bool>(r, true));
	}

}
void Graphics::Disable(RENDERING_TYPE r) const
{
	auto it = _renderingSettings.find(r);
	if (it != _renderingSettings.end())
	{
		if (it->second)
		{
			glDisable(r);
		}
		it->second = false;
	}
	else
	{
		glDisable(r);
		_renderingSettings.insert(std::pair<fm::RENDERING_TYPE, bool>(r, false));
	}
}

void Graphics::Draw(int primitiveType,
                    size_t vertexCount,
                    size_t* indices) const
{
    glDrawElements(GL_TRIANGLES, (GLsizei)vertexCount, GL_UNSIGNED_INT, (void*)indices);
}

void Graphics::Draw(int primitiveType,
                    size_t vertexStart,
                    size_t vertexCount) const
{
    glDrawArrays(GL_TRIANGLES, (GLsizei)vertexStart, (GLsizei)vertexCount);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Graphics::Draw(Model* model) const
{
    model->PrepareBuffer();
    for(size_t i = 0; i < model->GetNumberMeshes(); ++i)
    {
        fm::rendering::MeshContainer* mesh = model->GetMeshContainer(i);
        Draw((size_t)0, mesh->listIndices.size(), (size_t*)mesh->listIndices.data());
    }

}

// TODO VAO needed with opengl CORE and not ES
void Graphics::BindVertexBuffer(VertexBuffer* vertexBuffer) const
{
    vertexBuffer->prepareData();
}

void Graphics::BindFrameBuffer(unsigned int id) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Graphics::BindTexture2D(size_t number, int idTexture, int type) const
{
    glActiveTexture(GL_TEXTURE0 + (GLenum)number);
    glBindTexture(type, idTexture);
}

void Graphics::RestoreSettings(const RenderingSettings &inSettings)
{
	_renderingSettings = inSettings;
}

const RenderingSettings& Graphics::GetRenderingSettings()
{
	return _renderingSettings;
}


