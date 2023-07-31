#ifndef ENTITY_H
#define ENTITY_H
#include <vector>
#include "Component.h"


class Entity
{

public:

	struct Id {
		Id() : _id(0) {}
		explicit Id(uint64_t id) : _id(id) {}
		Id(uint32_t index, uint32_t version) : _id(uint64_t(index) | uint64_t(version) << 32UL) {}

		uint64_t id() const { return _id; }

		bool operator == (const Id& other) const { return _id == other._id; }
		bool operator != (const Id& other) const { return _id != other._id; }
		bool operator < (const Id& other) const { return _id < other._id; }

		uint32_t index() const { return _id & 0xffffffffUL; }
		uint32_t version() const { return _id >> 32; }

	private:
		friend class EntityManager;

		uint64_t _id;
	};

	static const Id INVALID;

    Entity();
    Entity(EntityManager* inManager, Id);
    ~Entity();


	template <typename T, typename ...Args> T* addComponent(Args&&... args);

	template <typename T> T* addEmpty();
	template <typename T> T* add(Component<T> *c);
	template <typename T> T* get();
	template <typename T> bool has();
	template <typename T> bool remove();
	std::vector<BaseComponent*> getAllComponents();
	bool Valid();
    
    void destroy();
	Id id() const { return _id; }

private:
	EntityManager* _manager = nullptr;
	Entity::Id _id = INVALID;

};

#endif
