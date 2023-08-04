#include <iostream>
#include "EntityManager.h"
#include "System.h"
#include "ComponentManager.h"
#include "SystemManager.h"
#include "Entity.h"
#include <chrono>
#include <cstring>
#include "Event.h"

class Position : public Component<Position> {
public:
	Position() {
		_name = "Position";
	}
	int x = 0;
	int y = 0;

	~Position() {
	}

	bool Serialize(nlohmann::json& ioJson) const
	{
		return false;
	}
	bool Read(const nlohmann::json& inJSON)
	{
		return false;
	}
	void Destroy() {

	}

};



class Movement : public System<Movement> {
public:
	Movement() {
	}
	void Start() {

	}
	void Stop() {

	}
	void update(float dt, EntityManager& em, EventManager& event) {
		for (auto e : em.iterate<Position>()) {
			auto temp = em.createEntity();
			temp.addComponent<Position>();

			auto position = e.get<Position>();
			
			std::cout << e.id().index() << std::endl;
		}
		// std::cout << "Called m" << std::endl;
		// std::cout << e->get<Position>()->x << std::endl;
	}
	void pre_update(EntityManager& em) {
		// std::cout << "Movement" << std::endl;
	}
	void over() {
	}
	~Movement() {
	}
	void init(EntityManager& em, EventManager& event) {
		//  std::cout << "Init movement" << std::endl;
		// Entity *en = EntityManager::get().createEntity();
		//  std::cout << en->ID << std::endl;
		//  en->addComponent<Position>();
	}
};


int main() {

	EntityManager &manager = EntityManager::get();

	{
		Entity e = manager.createEntity();
		e.addComponent<Position>();
	}
	
	{
		Entity e = manager.createEntity();
		e.addComponent<Position>();
	}

	SystemManager systemManager;
	systemManager.init(manager, EventManager::Get());

	systemManager.addSystem(new Movement);

	systemManager.Start();
	systemManager.update(1, manager, EventManager::Get());

    return 0;
}
