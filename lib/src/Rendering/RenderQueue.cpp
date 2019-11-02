#include "Rendering/RenderQueue.h"
#include <iostream>
#include <algorithm>
using namespace fm;

RenderQueue::RenderQueue() 
{
}

void RenderQueue::addElement(const RenderNode& node) 
{
    size_t i = node.state * _knumberElementsBetweenStates + node.queue;
    if(!_bits.test(i)) 
	{
        _elements.push_back(i);
        _bits.set(i, true);
    }
    _nodes[node.state * _knumberElementsBetweenStates + node.queue].push_back(node);
}

void RenderQueue::init() 
{
    for(auto && element : _elements) 
	{
        _nodes[element].clear();
    }

    _elements.clear();
    _currentState = FIRST_STATE;
    _currentQueue = 0;
    _currentIndex = 0;
    _currentIndexGlobal = 0;
    _sizeCurrentVector = 0;

    _indexElements = 0;
    _bits.reset();
}

RenderQueue::~RenderQueue() 
{
}

void RenderQueue::addElement(RenderNode&& node)
{
    size_t i = node.state * _knumberElementsBetweenStates + node.queue;
    if(!_bits.test(i)) 
	{
        _elements.push_back(i);
        _bits.set(i, true);
    }
    _nodes[node.state * _knumberElementsBetweenStates + node.queue].push_back(node);
}
RenderNode RenderQueue::getFrontElement() const 
{
    return _nodes[_currentIndexGlobal][_currentIndex];
}

void RenderQueue::nextState() 
{
    _currentState++;
}
void RenderQueue::nextQueue()
{
    _currentQueue++;
}

void RenderQueue::nextIndex() 
{
    _currentIndex++;
}

void RenderQueue::start() 
{
    
    std::sort(_elements.begin(), _elements.end());
    _currentIndexGlobal = _elements[_indexElements];
}

void RenderQueue::next() 
{

    _sizeCurrentVector = _nodes[_currentIndexGlobal].size();
    _currentIndex++;

    if(_currentIndex >= _sizeCurrentVector) 
	{
        _currentIndex = 0;
        _indexElements++;

        if(_indexElements < _elements.size()) 
		{
            _currentIndexGlobal = _elements[_indexElements];
        }
    }
}