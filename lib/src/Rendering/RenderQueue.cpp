#include "Rendering/RenderQueue.h"
#include <iostream>
#include <algorithm>
#include "Model.hpp"
#include "material.hpp"
using namespace fm;

RenderQueue::RenderQueue() 
{
}

void RenderQueue::AddElement(const RenderNode& node) 
{
    _nodes[node.state].push_back(node);
}

void RenderQueue::Init() 
{
	std::for_each(_nodes.begin(), _nodes.end(), [](std::vector<RenderNode>& inList) {inList.clear(); });
}

RenderQueue::~RenderQueue() 
{
}

int64_t RenderNode::Compare(const RenderNode& inNodeA, const RenderNode& inNodeB)
{
	uint32_t modelAID = 0;
	uint32_t modelBID = 0;
	uint32_t materialAID = 0;
	uint32_t materialBID = 0;

	if (std::shared_ptr<fm::Model> modelA = inNodeA.model.lock())
	{
		modelAID = modelA->GetID();
	}

	if (std::shared_ptr<fm::Model> modelB = inNodeB.model.lock())
	{
		modelBID = modelB->GetID();
	}

	if (std::shared_ptr<fm::Material> materialA = inNodeA.material.lock())
	{
		materialAID = materialA->GetID();
	}

	if (std::shared_ptr<fm::Material> materialB = inNodeB.material.lock())
	{
		materialBID = materialB->GetID();
	}


	if (modelAID == modelBID)
	{
		return (materialAID - materialBID);
	}
	return (modelAID - modelBID);
}


void RenderQueue::Sort() 
{
	std::for_each(_nodes.begin(), _nodes.end(),
		[](std::vector<RenderNode>& inList) {
			std::sort(inList.begin(), inList.end(),
			[](const RenderNode& inNodeA, const RenderNode& inNodeB)
			{
				return RenderNode::Compare(inNodeA, inNodeB);
			});
	});
}

RenderQueue::Iterator RenderQueue::Iterate()
{
	return Iterator(_nodes, RENDER_QUEUE::FIRST_STATE, 0, 0);
}

RenderQueue::Iterator::Iterator(const ArrayNode& inArray, RENDER_QUEUE inCurrentState, uint32_t inCurrentBaseBatch, uint32_t inCurrentGlobalBaseBatch)
{
	_currentBaseBatch = inCurrentBaseBatch;
	_currentState = inCurrentState;
	_array = inArray;
	_currentGlobalBaseBatch = inCurrentGlobalBaseBatch;
	next();
}

RenderQueue::Batch RenderQueue::Iterator::operator*()
{
	return _currentBatch;
}
RenderQueue::Iterator RenderQueue::Iterator::operator++()
{
	next();
	return Iterator(_array, (RENDER_QUEUE)_currentState, _currentBaseBatch, _currentGlobalBaseBatch);
}
RenderQueue::Iterator RenderQueue::Iterator::begin()
{
	return Iterator(_array, RENDER_QUEUE::FIRST_STATE, 0, 0);
}
RenderQueue::Iterator RenderQueue::Iterator::end()
{
	return Iterator(_array, RENDER_QUEUE::LAST_STATE, _array[fm::RENDER_QUEUE::LAST_STATE - 1].size(), 0);
}
bool RenderQueue::Iterator::operator!=(Iterator& i)
{
	return _currentState != i._currentState || _currentBaseBatch != i._currentBaseBatch;
}

void RenderQueue::Iterator::next()
{
	_currentBatch.baseInstance = _currentGlobalBaseBatch;
	_currentBatch.number = 0;

	uint32_t firstState = fm::RENDER_QUEUE::LAST_STATE;
	uint32_t sizeBatch = 0;
	bool hasFound = false;
	while (!hasFound)
	{
		for (firstState = _currentState; firstState < fm::RENDER_QUEUE::LAST_STATE; ++firstState)
		{
			if (!_array[firstState].empty())
			{
				break;
			}
		}

		if (firstState == fm::RENDER_QUEUE::LAST_STATE)
		{
			_currentState = firstState;
			break;
		}
			
		RenderNode firstNode{};
		bool hasFoudNode = false;
		if (_currentBaseBatch < _array[firstState].size())
		{
			hasFoudNode = true;
			firstNode = _array[firstState][_currentBaseBatch];
		}

		if (!hasFoudNode)
		{
			_currentBaseBatch = 0;
			_currentState+= 1;
			continue;
		}
		uint32_t baseBatch = 0;
		_currentBatch.node = RenderNode(firstNode);
		for (baseBatch = _currentBaseBatch; baseBatch < _array[firstState].size(); ++baseBatch)
		{
			RenderNode currentNode = _array[firstState][baseBatch];
			if (RenderNode::Compare(firstNode, currentNode) == 0)
			{
				sizeBatch++;
			}
			else
			{
				break;
			}
		}
		_currentBatch.number = sizeBatch;
		_currentBaseBatch = baseBatch;
		_currentGlobalBaseBatch += sizeBatch;
		_currentState = firstState;

		if (_currentState >= fm::RENDER_QUEUE::LAST_STATE)
			break;

		if (sizeBatch > 0)
			hasFound = true;
	}

}
