#pragma once
#include <Component.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "RenderTexture.h"
#include <iostream>
namespace fmc {
struct ViewPortCamera {
    float x;
    float y;
    float width;
    float height;
};

struct Shader_data
{
    glm::mat4 FM_V;
    glm::mat4 FM_P;
    glm::mat4 FM_PV;
};
class CCamera : public Component<CCamera> {
public:
    CCamera() {
    }
    CCamera(int width, int height) {
        projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, 0.0f, 100.0f);
        viewPort.width = width;
        viewPort.height = height;
        viewPort.x = 0;
        viewPort.y = 0;
        if(renderTexture.isCreated()) {
            renderTexture.release();
        }
        renderTexture = fm::RenderTexture(width, height,8);
    }
    ~CCamera() {
        if(renderTexture.isCreated()) renderTexture.release();
    }

    void setNewProjection(unsigned int width, unsigned int height) {
        projection = glm::ortho(0.0f, (float)width,  (float)height, 0.0f, 0.0f, 100.0f);
        viewPort.width = width;
        viewPort.height = height;
        viewPort.x = 0;
        viewPort.y = 0;
        if(renderTexture.isCreated()) renderTexture.release();

        renderTexture = fm::RenderTexture(width, height,8);

    }
    
    void updateRenderTexture() {
        if(viewPort.width != renderTexture.getWidth() 
        || viewPort.height != renderTexture.getHeight()) {
            renderTexture.release();
            renderTexture = fm::RenderTexture(viewPort.width, viewPort.height);
        }
    }
    
    void setNewViewPort(int x, int y, unsigned int width, unsigned int height) {
        projection = glm::ortho(0.0f, (float)width,  (float)height, 0.0f, 0.0f, 100.0f);
        viewPort.width = width;
        viewPort.height = height;
        viewPort.x = x;
        viewPort.y = y;
        if(renderTexture.isCreated()) renderTexture.release();

        renderTexture = fm::RenderTexture(width, height,8);

    }
    
    fm::RenderTexture& getRenderTexture() {
        return renderTexture;
    }
static const std::string name;


    ViewPortCamera viewPort;
    glm::mat4 projection;
    glm::mat4 viewMatrix;
    
    Shader_data shader_data;
    fm::RenderTexture renderTexture;
};
}