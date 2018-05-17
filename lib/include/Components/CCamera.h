#pragma once
#include "component.h"


#include "Core/Math/Matrix.h"
#include "Core/Rect.h"

namespace fm {
class RenderTexture;
}

namespace fms {
class RenderingSystem;
}
namespace fmc {

enum RENDER_MODE {
    FORWARD,
    DEFERRED

};

struct Shader_data {
        fm::math::mat FM_V;
        fm::math::mat FM_P;
        fm::math::mat FM_PV;
        int render_mode = fmc::RENDER_MODE::FORWARD;
};
class CCamera : public FMComponent<CCamera> {
    public:
        friend class fms::RenderingSystem;


        bool  isOrthographic();
        float getFarPlane();
        float getNearPlane();

        CCamera();
        CCamera(int width, int height, fmc::RENDER_MODE mode);
        ~CCamera();

        void setNewProjection(unsigned int width, unsigned int height);
        void updateRenderTexture();
        void setNewViewPort(int x, int y, unsigned int width, unsigned int height);


        fm::Rect<float> viewPort;
        fm::math::mat projection;

        Shader_data shader_data;
        std::shared_ptr<fm::RenderTexture> target = nullptr;
        std::shared_ptr<fm::RenderTexture> getInternalRenderTexture() {return renderTexture;}
    private:
        std::shared_ptr<fm::RenderTexture> renderTexture = nullptr;
        fm::math::mat viewMatrix;
        bool  isOrto = false;
        float farPlane = 100.0f;
        float nearPlane = 0.0f;
};
}
