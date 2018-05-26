#include "Components/CCamera.h"
#include "Rendering/RenderTexture.h"
using namespace fmc;
using namespace fm;

bool CCamera::IsOrthographic() {
    return _isOrto;
}

float CCamera::GetFarPlane() {
    return _farPlane;
}

float CCamera::GetNearPlane() {
    return _nearPlane;
}

CCamera::CCamera()
{
    _name = "Camera";
}

bool CCamera::Serialize(json &ioJson) const
{
    ioJson["width"] = viewPort.w;
    ioJson["height"] = viewPort.h;
    ioJson["farPlane"] = _farPlane;
    ioJson["nearPlane"] = _nearPlane;

    ioJson["Render_Mode"] = shader_data.render_mode;
    return true;
}
bool CCamera::Read(const json &inJSON)
{
    _width = inJSON["width"];
    _height = inJSON["height"];
    _farPlane =  inJSON["farPlane"];
    _nearPlane = inJSON["nearPlane"];
    shader_data.render_mode = inJSON["Render_Mode"];
    return true;
}

void CCamera::Init()
{
    _name = "Camera";

    _isOrto = true;
    // projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, 0.0f, 100.0f);
    projection = fm::math::ortho(0.0f, (float)_width, (float)_height, 0.0f, _nearPlane, _farPlane);
    viewPort.w = _width;
    viewPort.h = _height;
    viewPort.x = 0;
    viewPort.y = 0;


    if(_renderTexture != nullptr && _renderTexture->isCreated())
    {
        _renderTexture->release();
    }

    if(shader_data.render_mode == fmc::RENDER_MODE::DEFERRED)
    {
        fm::Format formats[] = { fm::Format::RGBA, fm::Format::RGBA, fm::Format::RGB };
#if OPENGL_ES_VERSION > 2
        fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE, fm::Type::HALF_FLOAT };
#else
        fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE };
#endif
        _renderTexture = std::make_shared<fm::RenderTexture>(_width, _height, 3, formats, types, 24);
    }
    else if(shader_data.render_mode == fmc::RENDER_MODE::FORWARD)
    {
        fm::Format formats[] = { fm::Format::RGBA, fm::Format::RGBA };

        fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE };
        _renderTexture = std::make_shared<fm::RenderTexture>(_width, _height, 2, formats, types, 24);
    }
}

CCamera::CCamera(int width, int height, fmc::RENDER_MODE mode)
{
    _width = width;
    _height = height;
    shader_data.render_mode = mode;
    Init();
}

CCamera::~CCamera()
{
    if(_renderTexture != nullptr && _renderTexture->isCreated())
    {
        _renderTexture->release();
    }
}

void CCamera::SetNewProjection(unsigned int width, unsigned int height)
{
    _width = width;
    _height = height;
    _isOrto = true;
    _farPlane = 100.0f;

    projection = fm::math::ortho(0.0f, (float)width, (float)height, 0.0f, _nearPlane, _farPlane);
    viewPort.w = width;
    viewPort.h = height;
    viewPort.x = 0;
    viewPort.y = 0;
    if(_renderTexture != nullptr && _renderTexture->isCreated())
    {
        _renderTexture->release();
    }

    _renderTexture = std::make_shared<fm::RenderTexture>(width, height, 8);
}

void CCamera::UpdateRenderTexture()
{
    if(_renderTexture != nullptr)
    {
        if(viewPort.w != _renderTexture->getWidth() || viewPort.h != _renderTexture->getHeight())
        {
            _renderTexture->release();
            if(shader_data.render_mode == fmc::RENDER_MODE::DEFERRED) {
                fm::Format formats[] = { fm::Format::RGBA, fm::Format::RGBA, fm::Format::RGB };

#if OPENGL_ES_VERSION > 2
                fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE, fm::Type::HALF_FLOAT };
#else
                fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE };
#endif
                _renderTexture = std::make_shared<fm::RenderTexture>(viewPort.w, viewPort.h, 3, formats, types, 24);
            } else if(shader_data.render_mode == fmc::RENDER_MODE::FORWARD) {
                fm::Format formats[] = { fm::Format::RGBA, fm::Format::RGBA };

                fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE };
                _renderTexture = std::make_shared<fm::RenderTexture>(viewPort.w, viewPort.h, 3, formats, types, 24);
            }
        }
    }
}

void CCamera::SetNewViewPort(int x, int y, unsigned int width, unsigned int height)
{
    _isOrto = true;
    projection = fm::math::ortho((float)x, (float)x + (float)width, (float)y + (float)height, (float)y, _nearPlane, _farPlane);
    viewPort.w = width;
    viewPort.h = height;
    viewPort.x = x;
    viewPort.y = y;
    if(_renderTexture != nullptr && _renderTexture->isCreated())
    {
        _renderTexture->release();
    }
    if(shader_data.render_mode == fmc::RENDER_MODE::DEFERRED)
    {
        fm::Format formats[] = { fm::Format::RGBA, fm::Format::RGBA, fm::Format::RGB };

#if OPENGL_ES_VERSION > 2
        fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE, fm::Type::HALF_FLOAT };
#else
        fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE };
#endif
        _renderTexture = std::make_shared<fm::RenderTexture>(width, height, 3, formats, types, 24);
    }
    else if(shader_data.render_mode == fmc::RENDER_MODE::FORWARD)
    {
        fm::Format formats[] = { fm::Format::RGBA, fm::Format::RGBA };

        fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE };
        _renderTexture = std::make_shared<fm::RenderTexture>(width, height, 2, formats, types, 24);
    }
}

