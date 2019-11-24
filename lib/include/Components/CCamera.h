#pragma once
#include "component.h"
#include "Rendering/commandBuffer.hpp"

#include "Core/Math/Matrix.h"
#include "Core/Rect.h"
#include "Rendering/RenderQueue.h"
#include "Rendering/uniformbuffer.hpp"
#include "Rendering/RenderTexture.h"
#include <mutex>

namespace fm 
{
	class RenderTexture;
	struct Transform;
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

	fm::RenderTexture lightRenderTexture;
	fm::RenderTexture postProcessRenderTexture;

	fm::Bounds bounds;
	std::unique_ptr<fm::UniformBuffer> uboLight;
};

typedef std::unordered_map<fm::RENDER_QUEUE, std::queue<fm::CommandBuffer>> CameraCommandBuffer;

class CCamera : public FMComponent<CCamera> 
{
    public:
        friend class fms::RenderingSystem;

		CCamera();
        CCamera(int width, int height, fmc::RENDER_MODE mode, bool ortho, bool isAuto = false, int multiSampled = 0);
        ~CCamera();

        bool  IsOrthographic();
        float GetFarPlane();
        float GetNearPlane();

        bool Serialize(nlohmann::json &ioJson) const override;
        bool Read(const nlohmann::json &inJSON) override;
		uint16_t GetType() const override {return kCamera;}
        void Destroy() override;

        void SetNewProjection(unsigned int width, unsigned int height);
        void UpdateRenderTexture();
        void SetNewViewPort(int x, int y, unsigned int width, unsigned int height);
        void Init();



        Shader_data shader_data;
        std::shared_ptr<fm::RenderTexture> target = nullptr;
        const fm::RenderTexture &getInternalRenderTexture() const {return _renderTexture;}
		bool IsInit() { return _renderTexture.isCreated() && _isInit; }
		void AddCommandBuffer(fm::RENDER_QUEUE inQueue, const fm::CommandBuffer &inCommandBuffer);

		void SetCallBackOnStartRendering(std::function<void()> && inCallback) { _onStartRendering = inCallback; }
		void SetCallBackOnPostRendering(std::function<void()> && inCallback) { _onPostRendering = inCallback; }

		void UpdateProjectionMatrix();
		void UpdateViewMatrix(const fm::Transform &inTransform);
		void UpdateShaderData();

		const fm::Rect<float>& GetViewport()const { return _viewPort; }
		const fm::math::mat& GetProjectionMatrix()const { return _projection; }
		const fm::math::mat& GetViewMatrix() const { return _viewMatrix; }
		const RendererConfiguration& GetRendererConfig() const { return _rendererConfiguration; }
		void InitRenderConfig(const fm::Transform &inTransform, size_t sizeBytesLight);
		void InitUniformBuffer();
		void UpdateUniformBufferCamera();
		void UpdateRenderConfigBounds(const fm::Transform &inTransform);

		const fm::RenderTexture& GetRenderTexture() const { return _renderTexture; }
    private:
		std::function<void()> _onStartRendering = nullptr;
		std::function<void()> _onPostRendering = nullptr;
		bool HasCommandBuffer(fm::RENDER_QUEUE inQueue) const;

		void _InitRenderTexture();
		RendererConfiguration	_rendererConfiguration;

        fm::RenderTexture		_renderTexture;
        bool					_isOrto;
        float					_farPlane;
        float					_nearPlane;
        int						_multiSampled;
        unsigned int			_width;
        unsigned int			_height;
        float					_fovy;
		bool					_isAuto;

		CameraCommandBuffer		_commandBuffers;
		bool					_isInit;

		fm::Rect<float>			_viewPort;
		fm::math::mat			_projection;
		fm::math::mat			_viewMatrix;
};
}
