#pragma once


#include <queue>
#include <vector>
#include <bitset>
#include "Rendering/RenderQueueEvents.hpp"
#include "Core/Transform.h"

namespace fm
{
	class Model;
	class Material;
}

namespace fmc
{
	class CDirectionalLight;
	class CPointLight;
	class CText;
}

namespace fm
{
	using Materials = std::vector<fm::Material*>;
}

namespace fm {

struct RenderNode {
    const fm::Transform transform;
    const fm::Materials *mat;
		  fm::Model *model;
    const fmc::CDirectionalLight *dlight;
    const fmc::CPointLight *plight;
    const fmc::CText *text;
    
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
    inline size_t Size() const
    {
        return _elements.size();
    }

    inline bool Empty() const
    {
        return _indexElements >= _elements.size() || _elements.size() == 0;
    }
    
    private:
    const static size_t _knumberElementsBetweenStates = 10;
    std::array< std::vector<RenderNode>, RENDER_QUEUE::LAST_STATE * _knumberElementsBetweenStates> _nodes;
    std::vector<size_t> _elements;
    std::bitset<RENDER_QUEUE::LAST_STATE * _knumberElementsBetweenStates> _bits;
    
    size_t _currentState = FIRST_STATE;
    size_t _currentQueue = 0;
    size_t _currentIndex = 0;
    size_t _currentIndexGlobal = 0;
    size_t _sizeCurrentVector = 0;
    
    size_t _indexElements = 0;
    bool e = false;
};

}
