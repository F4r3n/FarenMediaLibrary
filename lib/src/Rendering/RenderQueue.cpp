#include "Rendering/RenderQueue.h"
#include <iostream>
#include <algorithm>
using namespace fm;

RenderQueue::RenderQueue() {
}

void RenderQueue::addElement(const RenderNode& node) {
    int i = (int)node.state * numberElementsBetweenStates + node.queue;
    if(!bits.test(i)) {
        elements.push_back(i);
        bits.set(i, true);
    }
    nodes[(int)node.state * numberElementsBetweenStates + node.queue].push_back(node);
}

void RenderQueue::init() {
    for(unsigned int i = 0; i < elements.size(); ++i) {
        nodes[elements[i]].clear();
    
    }
    elements.clear();
    currentState = FIRST_STATE;
    currentQueue = 0;
    currentIndex = 0;
    currentIndexGlobal = 0;
    sizeCurrentVector = 0;

    indexElements = 0;
    bits.reset();
}

RenderQueue::~RenderQueue() {
}

void RenderQueue::addElement(RenderNode&& node) {
    int i = (int)node.state * numberElementsBetweenStates + node.queue;
    if(!bits.test(i)) {
        elements.push_back(i);
        bits.set(i, true);
    }
    nodes[(int)node.state * numberElementsBetweenStates + node.queue].push_back(node);
}
RenderNode RenderQueue::getFrontElement() const {
    return nodes[currentIndexGlobal][currentIndex];
}

void RenderQueue::nextState() {
    currentState++;
}
void RenderQueue::nextQueue() {
    currentQueue++;
}

void RenderQueue::nextIndex() {
    currentIndex++;
}

void RenderQueue::start() {
    std::sort(elements.begin(), elements.end());
    currentIndexGlobal = elements[indexElements];
}

void RenderQueue::next() {

    sizeCurrentVector = nodes[currentIndexGlobal].size();
    currentIndex++;
    if(currentIndex >= sizeCurrentVector) {
        currentIndex = 0;
        indexElements++;
        if(indexElements < elements.size()) {
            currentIndexGlobal = elements[indexElements];
        }
    }
}