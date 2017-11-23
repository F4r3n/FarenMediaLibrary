#pragma once
#include <Component.h>
#include "Rendering/RenderTexture.h"

#include "Core/Math/Matrix.h"
#include "Core/Rect.h"
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
    class CCamera : public Component<CCamera> {
        public:
            bool isOrthographic() {
                return isOrto;
            }
            int* get(int v) {return nullptr;}

            float getFarPlane() {
                return farPlane;
            }

            float getNearPlane() {
                return nearPlane;
            }

            CCamera() {
                _name = "Camera";
            }
            CCamera(int width, int height, fmc::RENDER_MODE mode) {
                _name = "Camera";

                isOrto = true;
                // projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, 0.0f, 100.0f);
                projection = fm::math::ortho(0.0f, (float)width, (float)height, 0.0f, nearPlane, farPlane);
                viewPort.w = width;
                viewPort.h = height;
                viewPort.x = 0;
                viewPort.y = 0;

                if(renderTexture != nullptr && renderTexture->isCreated()) {
                    renderTexture->release();
                }
                shader_data.render_mode = mode;
                if(shader_data.render_mode == fmc::RENDER_MODE::DEFERRED) {
                    fm::Format formats[] = { fm::Format::RGBA, fm::Format::RGBA, fm::Format::RGB };
#if OPENGL_ES_VERSION > 2
                    fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE, fm::Type::HALF_FLOAT };
#else
                    fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE };
#endif
                    renderTexture = std::make_shared<fm::RenderTexture>(width, height, 3, formats, types, 24);
                } else if(shader_data.render_mode == fmc::RENDER_MODE::FORWARD) {
                    fm::Format formats[] = { fm::Format::RGBA, fm::Format::RGBA };

                    fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE };
                    renderTexture = std::make_shared<fm::RenderTexture>(width, height, 2, formats, types, 24);
                }
            }
            ~CCamera() {
                if(renderTexture != nullptr && renderTexture->isCreated()) {
                    renderTexture->release();
                }
            }

            void setNewProjection(unsigned int width, unsigned int height) {
                isOrto = true;
                farPlane = 100.0f;

                projection = fm::math::ortho(0.0f, (float)width, (float)height, 0.0f, nearPlane, farPlane);
                viewPort.w = width;
                viewPort.h = height;
                viewPort.x = 0;
                viewPort.y = 0;
                if(renderTexture != nullptr && renderTexture->isCreated()) {
                    renderTexture->release();
                }

                renderTexture = std::make_shared<fm::RenderTexture>(width, height, 8);
            }

            void updateRenderTexture() {
                if(renderTexture != nullptr) {
                    if(viewPort.w != renderTexture->getWidth() || viewPort.h != renderTexture->getHeight()) {
                        renderTexture->release();
                        if(shader_data.render_mode == fmc::RENDER_MODE::DEFERRED) {
                            fm::Format formats[] = { fm::Format::RGBA, fm::Format::RGBA, fm::Format::RGB };

#if OPENGL_ES_VERSION > 2
                            fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE, fm::Type::HALF_FLOAT };
#else
                            fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE };
#endif
                            renderTexture = std::make_shared<fm::RenderTexture>(viewPort.w, viewPort.h, 3, formats, types, 24);
                        } else if(shader_data.render_mode == fmc::RENDER_MODE::FORWARD) {
                            fm::Format formats[] = { fm::Format::RGBA, fm::Format::RGBA };

                            fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE };
                            renderTexture = std::make_shared<fm::RenderTexture>(viewPort.w, viewPort.h, 3, formats, types, 24);
                        }
                    }
                }
            }

            void setNewViewPort(int x, int y, unsigned int width, unsigned int height) {
                isOrto = true;
                projection =
                    fm::math::ortho((float)x, (float)x + (float)width, (float)y + (float)height, (float)y, nearPlane, farPlane);
                viewPort.w = width;
                viewPort.h = height;
                viewPort.x = x;
                viewPort.y = y;
                if(renderTexture != nullptr && renderTexture->isCreated()) {
                    renderTexture->release();
                }
                if(shader_data.render_mode == fmc::RENDER_MODE::DEFERRED) {
                    fm::Format formats[] = { fm::Format::RGBA, fm::Format::RGBA, fm::Format::RGB };

#if OPENGL_ES_VERSION > 2
                    fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE, fm::Type::HALF_FLOAT };
#else
                    fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE };
#endif
                    renderTexture = std::make_shared<fm::RenderTexture>(width, height, 3, formats, types, 24);
                } else if(shader_data.render_mode == fmc::RENDER_MODE::FORWARD) {
                    fm::Format formats[] = { fm::Format::RGBA, fm::Format::RGBA };

                    fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE };
                    renderTexture = std::make_shared<fm::RenderTexture>(width, height, 2, formats, types, 24);
                }
            }

            std::shared_ptr<fm::RenderTexture> getRenderTexture() {
                return renderTexture;
            }
            static const std::string name;

            fm::Rect<float> viewPort;
            fm::math::mat projection;
            fm::math::mat viewMatrix;

            Shader_data shader_data;
            std::shared_ptr<fm::RenderTexture> renderTexture = nullptr;

        private:
            bool isOrto = false;
            float farPlane = 100.0f;
            float nearPlane = 0.0f;
    };
}
