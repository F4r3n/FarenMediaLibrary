#pragma once

#include <functional>
#include <queue>
#include <vector>
#include "Components/CMaterial.h"
#include "Components/CMesh.h"
#include "Components/CTransform.h"
#include "Components/CDirectionalLight.h"
#include "Components/CPointLight.h"
#include "Components/CText.h"

namespace fm {
enum RENDER_QUEUE { BACKGROUND = 1000, OPAQUE = 2000, LIGHT = 3000, AFTER_LIGHT = 3500,TRANSPARENT = 4000, SKYBOX = 5000, OVERLAY = 6000 };

struct RenderNode {
    fmc::CTransform *transform;
    fmc::CMaterial *mat;
    fmc::CMesh *mesh;
    fmc::CDirectionalLight *dlight;
    fmc::CPointLight *plight;
    fmc::CText *text;
    
    int queue;

};
    inline bool operator<(const RenderNode& a, const RenderNode &b){
        return a.queue > b.queue; //Inverse heap
    }
class RenderQueue {
public:
    RenderQueue();
    ~RenderQueue();
    void addElement(RenderNode &&node);
    void addElement(const RenderNode &node);
    RenderNode getFrontElement() const;
    void removeFront();
    inline bool empty() const{
        return nodes.empty();
    }
    
private:
    std::priority_queue<RenderNode> nodes;
};

}