#pragma once
#include <Component.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Rendering/RenderTexture.h"
#include <iostream>
namespace fmc {
struct ViewPortCamera {
    float x;
    float y;
    float width;
    float height;
};

enum RENDER_MODE {
    FORWARD,
    DEFERRED
};

struct Shader_data
{
    glm::mat4 FM_V;
    glm::mat4 FM_P;
    glm::mat4 FM_PV;
    int render_mode = fmc::RENDER_MODE::DEFERRED;
};
class CCamera : public Component<CCamera> {
public:
    CCamera() {
    }
    CCamera(int width, int height) {

        projection = glm::ortho(0.0f, (float)width,  (float)height, 0.0f, 0.0f, 100.0f);
        viewPort.width = width;
        viewPort.height = height;
        viewPort.x = 0;
        viewPort.y = 0;
        if(renderTexture != nullptr&& renderTexture->isCreated()) {
            renderTexture->release();
        }
        renderTexture = std::make_shared<fm::RenderTexture>(width, height,8);
    }
    ~CCamera() {
        if(renderTexture != nullptr && renderTexture->isCreated()) {
            renderTexture->release();
        }
    }

    void setNewProjection(unsigned int width, unsigned int height) {

        projection = glm::ortho(0.0f, (float)width,  (float)height, 0.0f, 0.0f, 100.0f);
        viewPort.width = width;
        viewPort.height = height;
        viewPort.x = 0;
        viewPort.y = 0;
        if(renderTexture != nullptr && renderTexture->isCreated()) {
            renderTexture->release();
        }

        renderTexture = std::make_shared<fm::RenderTexture>(width, height,8);

    }
    
    void updateRenderTexture() {
        if(renderTexture != nullptr) {
            if(viewPort.width != renderTexture->getWidth() 
            || viewPort.height != renderTexture->getHeight()) {
                renderTexture->release();
                renderTexture = std::make_shared<fm::RenderTexture>(viewPort.width, viewPort.height);
            }
            
        }
    }
    
    void setNewViewPort(int x, int y, unsigned int width, unsigned int height) {

        projection = glm::ortho((float)x, (float)x + (float)width,  (float)y + (float)height, (float)y, 0.0f, 100.0f);
        viewPort.width = width;
        viewPort.height = height;
        viewPort.x = x;
        viewPort.y = y;
        if(renderTexture != nullptr && renderTexture->isCreated()) {
            renderTexture->release();
        }
        renderTexture = std::make_shared<fm::RenderTexture>(width, height,8);

    }
    
    std::shared_ptr<fm::RenderTexture> getRenderTexture() {
        return renderTexture;
    }
static const std::string name;


    ViewPortCamera viewPort;
    glm::mat4 projection;
    glm::mat4 viewMatrix;
    
    Shader_data shader_data;
    std::shared_ptr<fm::RenderTexture> renderTexture = nullptr;
    
};
}