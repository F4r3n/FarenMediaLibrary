#include "Rendering/RenderQueue.h"


using namespace fm;
RenderQueue::RenderQueue() {
    
}

void RenderQueue::addElement(const RenderNode &node) {
        nodes.push(node);
}


RenderQueue::~RenderQueue() {
    
}

void RenderQueue::addElement(RenderNode &&node) {
    nodes.push(node);
}
RenderNode RenderQueue::getFrontElement() const{
    return nodes.top();
}

void RenderQueue::removeFront() {
    nodes.pop();
}