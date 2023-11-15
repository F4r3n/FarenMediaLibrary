#pragma once
#include "component.h"
#include "Core/Math/Matrix44.h"
#include "Core/Rect.h"
#include "Rendering/RenderTexture.hpp"
#include "Core/Bounds.h"
#include <mutex>
#include <queue>
#include <functional>
#include "Rendering/RenderQueueEvents.hpp"
#include "Object.hpp"
namespace fm
{
	class RenderTexture;
	struct Transform;
	class UniformBuffer;
	class CommandBuffer;
	class FrameBuffer;
}

namespace fms
{
	class RenderingSystem;
}
namespace fmc
{

	enum class RENDER_MODE
	{
		FORWARD,
		DEFERRED
	};

	struct Shader_data
	{
		fm::math::mat FM_V;
		fm::math::mat FM_P;
		fm::math::mat FM_PV;
	};

	typedef std::unordered_map<fm::RENDER_QUEUE, std::queue<fm::CommandBuffer>> CameraCommandBuffer;

	class CCamera : public FMComponent<CCamera>, public fm::Object<CCamera>
	{
	public:

		CCamera();
		CCamera(size_t width, size_t height, fmc::RENDER_MODE mode, bool ortho, bool isAuto = false, int multiSampled = 0);
		~CCamera();

		bool Serialize(nlohmann::json& ioJson) const override;
		bool Read(const nlohmann::json& inJSON) override;
		uint16_t GetType() const override { return kCamera; }

		void SetNewProjection(int width, int height);
		void UpdateRenderTexture();
		void SetNewViewPort(int x, int y, int width, int height);


		Shader_data shader_data;
		void AddCommandBuffer(fm::RENDER_QUEUE inQueue, const fm::CommandBuffer& inCommandBuffer);

		void SetCallBackOnStartRendering(std::function<void()>&& inCallback) { _onStartRendering = inCallback; }
		void SetCallBackOnPostRendering(std::function<void()>&& inCallback) { _onPostRendering = inCallback; }

		void UpdateProjectionMatrix();
		void UpdateViewMatrix(const fm::Transform& inTransform);

		const fm::Rect<int>& GetViewport()const { return _viewPort; }
		const fm::math::mat& GetProjectionMatrix()const { return _projection; }
		const fm::math::mat& GetViewMatrix() const { return _viewMatrix; }
		void InitUniformBuffer();
		void UpdateUniformBufferCamera();

		float GetFOV() const { return _fovy; }
		float GetFarPlane() const { return _farPlane; }
		float GetNearPlane() const { return _nearPlane; }
		bool  IsOrthographic() const { return _isOrto; }


		void SetFov(float inValue) { _fovy = inValue; }
		void SetFarPlane(float inValue) { _farPlane = inValue; }
		void SetNearPlane(float inValue) { _nearPlane = inValue; }
		void SetOrthoGraphic(bool value) { _isOrto = value; }

		void SetTarget(std::shared_ptr<fm::FrameBuffer> inFrameBuffer);

		std::shared_ptr<fm::FrameBuffer> GetTarget() const { return _target; }
		inline bool HasTarget() const { return _target != nullptr; }
		fm::math::mat GetOrthographicProjectionForText() const;
		inline bool IsAuto() const { return _isAuto; }
		inline CameraCommandBuffer& GetCommandBuffer() { return _commandBuffers; }
		void ExecuteStartRendering();

		void ExecutePostRendering();

		size_t GetWidth() const { return _width; }
		size_t GetHeight() const { return _height; }
		size_t GetMultiSampled() const { return _multiSampled; }

		size_t GetStamp() const { return _stamp; }

	private:
		std::function<void()> _destroyCallback = {};

		void _Touch() { _stamp++; }
		std::function<void()> _onStartRendering = nullptr;
		std::function<void()> _onPostRendering = nullptr;
		bool HasCommandBuffer(fm::RENDER_QUEUE inQueue) const;

		void _InitRenderTexture();

		std::shared_ptr<fm::RenderTexture> _renderTexture = nullptr;

		bool					_isOrto;
		float					_nearPlane;
		float					_farPlane;
		float					_fovy;
		size_t					_width;
		size_t					_height;
		int						_multiSampled;
		bool					_isInit;
		bool					_isAuto;
		std::shared_ptr<fm::FrameBuffer> _target = nullptr;

		CameraCommandBuffer		_commandBuffers;

		fm::Rect<int>			_viewPort;
		fm::math::mat			_projection;
		fm::math::mat			_viewMatrix;

		size_t					_stamp = 0;
	};
}
