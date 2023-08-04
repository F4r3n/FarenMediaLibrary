#pragma once
#include "Entity.h"
#include "ComponentManager.h"
#include <functional>
typedef BitSet Mask;
class EntityManager;

class EntityIteratorMask {
public:
	EntityIteratorMask(const Mask& mask, size_t inCursor, size_t inMax, std::function<bool(EntityManager&, const Entity::Id&)> inPredicate);

	Entity operator*();

	EntityIteratorMask operator++();

	EntityIteratorMask begin();

	EntityIteratorMask end();

	bool operator!=(EntityIteratorMask& i);

	void next();

	bool predicate();
	

private:
	std::function<bool(EntityManager&, const Entity::Id&)> _customPredicate = {};
	EntityManager* _manager;
	Mask _mask;
	uint32_t _cursor;
	size_t _max;

};