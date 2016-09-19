#pragma once
#include <Component.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace fmc
{
struct ViewPortCamera
{
    float x;
    float y;
    float width;
    float height;
};
class CCamera : public Component<CCamera>
{
    public:
    CCamera() {
        
    }
    CCamera(int width, int height)
    {
        projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
        viewPort.width = width;
        viewPort.height = height;
        viewPort.x = 0;
        viewPort.y = 0;
    }
    ~CCamera()
    {
    }
  
    ViewPortCamera viewPort;
    glm::mat4 projection;
};
}