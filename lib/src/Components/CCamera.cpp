#include "Components/CCamera.h"
using namespace fmc;
using namespace fm;

bool CCamera::isOrthographic() {
    return isOrto;
}

float CCamera::getFarPlane() {
    return farPlane;
}

float CCamera::getNearPlane() {
    return nearPlane;
}

CCamera::CCamera() {
    _name = "Camera";
}

CCamera::CCamera(int width, int height, fmc::RENDER_MODE mode) {
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
CCamera::~CCamera() {
    if(renderTexture != nullptr && renderTexture->isCreated()) {
        renderTexture->release();
    }
}

void CCamera::setNewProjection(unsigned int width, unsigned int height) {
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

void CCamera::updateRenderTexture() {
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

void CCamera::setNewViewPort(int x, int y, unsigned int width, unsigned int height) {
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

