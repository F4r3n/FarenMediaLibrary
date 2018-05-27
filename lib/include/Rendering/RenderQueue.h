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
#include <bitset>
namespace fm {
enum RENDER_QUEUE {FIRST_STATE,  BACKGROUND, OPAQUE, TRANSPARENT, 
LIGHT, AFTER_LIGHT, SKYBOX, OVERLAY, LAST_STATE};

struct RenderNode {
    fmc::CTransform *transform;
    fmc::CMaterial *mat;
    fmc::CMesh *mesh;
    fmc::CDirectionalLight *dlight;
    fmc::CPointLight *plight;
    fmc::CText *text;
    
    RENDER_QUEUE state;
    int queue = 0;
    size_t idEntity;
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
    void nextState();
    void nextQueue();
    void nextIndex();
    
    void next();
    void init();
    void start();
    inline size_t Size()
    {
        return elements.size();
    }

    inline bool Empty() const
    {
        return indexElements >= elements.size() || elements.size() == 0;
    }
    
    private:
    const static int numberElementsBetweenStates = 10;
    std::array< std::vector<RenderNode>, RENDER_QUEUE::LAST_STATE * numberElementsBetweenStates> nodes;
    std::vector<int> elements;
    std::bitset<RENDER_QUEUE::LAST_STATE * numberElementsBetweenStates> bits;
    
    unsigned int currentState = FIRST_STATE;
    unsigned int currentQueue = 0;
    unsigned int currentIndex = 0;
    unsigned int currentIndexGlobal = 0;
    unsigned int sizeCurrentVector = 0;
    
    unsigned int indexElements = 0;
    bool e = false;
};

}
