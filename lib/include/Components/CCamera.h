#pragma once
#include "component.h"


#include "Core/Math/Matrix.h"
#include "Core/Rect.h"
#include "Rendering/RenderQueue.h"
#include "Rendering/uniformbuffer.hpp"
namespace fm 
{
class RenderTexture;
}

namespace fms 
{
class RenderingSystem;
}
namespace fmc 
{

enum RENDER_MODE 
{
    FORWARD,
    DEFERRED

};

struct Shader_data 
{
        fm::math::mat FM_V;
        fm::math::mat FM_P;
        fm::math::mat FM_PV;
        int render_mode = fmc::RENDER_MODE::FORWARD;
};

struct RendererConfiguration
{
	bool isInit = false;
	const GLuint bindingPointIndex = 2;
	GLuint generatedBlockBinding;
	fm::RenderQueue queue;

	bool blendingMode = false;
	int queuePreviousValue = 0;

	std::shared_ptr<fm::RenderTexture> lightRenderTexture;
	std::shared_ptr<fm::RenderTexture> intermediate;

	fm::Bounds bounds;
	std::unique_ptr<fm::UniformBuffer> uboLight;
};



class CCamera : public FMComponent<CCamera> 
{
    public:
        friend class fms::RenderingSystem;

        CCamera();
        CCamera(int width, int height, fmc::RENDER_MODE mode, bool ortho, int multiSampled = 0);
        ~CCamera();

        bool  IsOrthographic();
        float GetFarPlane();
        float GetNearPlane();

        bool Serialize(nlohmann::json &ioJson) const override;
        bool Read(const nlohmann::json &inJSON) override;
        size_t GetType() const override {return kCamera;}
        void Destroy() override;

        void SetNewProjection(unsigned int width, unsigned int height);
        void UpdateRenderTexture();
        void SetNewViewPort(int x, int y, unsigned int width, unsigned int height);
        void Init();

        fm::Rect<float> viewPort;
        fm::math::mat projection;

        Shader_data shader_data;
        std::shared_ptr<fm::RenderTexture> target = nullptr;
        std::shared_ptr<fm::RenderTexture> getInternalRenderTexture() const {return _renderTexture;}
    private:

		RendererConfiguration _rendererConfiguration;

        std::shared_ptr<fm::RenderTexture> _renderTexture = nullptr;
        fm::math::mat _viewMatrix;
        bool  _isOrto = false;
        float _farPlane = 100.0f;
        float _nearPlane = 0.0f;
        int _multiSampled = 0;
        int _width;
        int _height;
        float _fovy = 60.0f;
};
}
