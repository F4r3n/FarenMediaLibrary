#pragma once 

namespace fmc {
    class CCamera;
}

struct CameraInfo {
    size_t ID;
    fmc::CCamera *camera;
};