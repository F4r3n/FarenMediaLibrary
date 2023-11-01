#pragma once


#include <vector>
#include "Rendering/RenderQueueEvents.hpp"
#include "Core/Transform.h"
#include <optional>
#include <memory>
#include <array>
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


namespace fm {

struct RenderNode
{
	RenderNode() = default;
	RenderNode(const RenderNode&) = default;
	fm::RenderNode& operator=(const RenderNode&) = default;
    fm::Transform transform;
    std::weak_ptr<fm::Material> material;
	std::weak_ptr<fm::Model> model;
	fmc::CText *text = nullptr;
    
    RENDER_QUEUE state;

	static int64_t Compare(const RenderNode& inRenderNodeA, const RenderNode& inRenderNodeB);
};

class RenderQueue {
public:

	struct Batch
	{
		RenderNode	node;
		uint32_t	baseInstance = 0;
		uint32_t	number = 0;
	};
	using ArrayNode = std::array< std::vector<RenderNode>, RENDER_QUEUE_LAST_STATE>;
	struct Iterator
	{
		Iterator(const ArrayNode& inArray, RENDER_QUEUE inCurrentState, uint32_t inCurrentBaseBatch, uint32_t inCurrentGlobalBaseBatch);
		Batch operator*();
		Iterator operator++();
		Iterator begin();
		Iterator end();
		bool operator!=(Iterator& i);
		void next();
	private:
		uint32_t		_currentState = RENDER_QUEUE_FIRST_STATE;
		uint32_t		_currentBaseBatch = 0;
		uint32_t		_currentGlobalBaseBatch = 0;
		ArrayNode		_array;
		Batch			_currentBatch;
	};
	Iterator Iterate();
    RenderQueue();
    ~RenderQueue();
    void AddElement(const RenderNode &node);

    void Init();
    void Sort();

    
    private:
    ArrayNode _nodes;
};

}
