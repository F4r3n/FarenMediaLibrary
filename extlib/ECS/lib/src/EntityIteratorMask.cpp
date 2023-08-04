#include "EntityIteratorMask.h"
#include "EntityManager.h"
EntityIteratorMask::EntityIteratorMask(const Mask& mask, size_t inCursor, size_t inMax, std::function<bool(EntityManager&, const Entity::Id&)> inPredicate) {
	_mask = mask;
	_cursor = inCursor;
	_manager = &EntityManager::get();
	_max = inMax;
	_customPredicate = inPredicate;
	next();
}

Entity EntityIteratorMask::operator*() {
	return Entity(_manager, _manager->CreateID(_cursor));
}

EntityIteratorMask EntityIteratorMask::operator++() {
	_cursor++;
	next();
	return EntityIteratorMask(_mask, _cursor, _max, _customPredicate);
}

EntityIteratorMask EntityIteratorMask::begin() {
	return EntityIteratorMask(_mask, 0, _max, _customPredicate);
}

EntityIteratorMask EntityIteratorMask::end() {
	return EntityIteratorMask(_mask, _max, _max, _customPredicate);
}

bool EntityIteratorMask::operator!=(EntityIteratorMask& i) {
	return _cursor != i._cursor;
}

void EntityIteratorMask::next() {

	while (_cursor < _max && !predicate())
	{
		++_cursor;
	}
}

bool EntityIteratorMask::predicate()
{
	Entity::Id id = _manager->CreateID(_cursor);
	if (_manager->Valid(id))
	{
		if (_manager->hasComponents(id, _mask))
		{
			bool ok = true;
			if (_customPredicate)
			{
				ok = _customPredicate(*_manager, id);
			}
			return ok;
		}
	}
	return false;
}