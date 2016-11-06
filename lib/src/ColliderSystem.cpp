#include "ColliderSystem.h"
#include "Components/CTransform.h"
#include "Entity.h"
#include <chrono>
#include "EventComponents.h"
#include <Event.h>
using namespace fms;

ColliderSystem::ColliderSystem() {
    
    quadTree = std::make_unique<fm::QuadTree>(0, fm::Recti(0, 0, 800, 600));
}
ColliderSystem::~ColliderSystem() {
}
void ColliderSystem::update(float dt, EntityManager& em, EventManager& event) {
    for(auto e : em.iterate<fmc::CCollider, fmc::CTransform>()) {
        //if(e->ID != 0) continue;
        fmc::CCollider* collider = e->get<fmc::CCollider>();
        fmc::CTransform* transform = e->get<fmc::CTransform>();
        fm::Recti r = fm::Recti(transform->position.x, transform->position.y, collider->rect.w, collider->rect.h);
        const std::vector<fm::Object>* objects = quadTree->getData(r);
        if(objects) {
            //std::cout << "Number Objects " << objects->size() << std::endl;
            for(fm::Object o : *objects) {
                //std::cout << o.rect << std::endl;
                if(o.rect.contains(r) && e->ID != o.idObject) {
                    event.emit<Collision>(o.idObject, e->ID);
              //      std::cout << e->ID << " " << o.idObject << std::endl;
                }
            }
        }
    }
}
void ColliderSystem::init(EntityManager& em, EventManager& event) {

    //   std::cout << elapsed_seconds.count() << std::endl;
}

void ColliderSystem::pre_update(EntityManager& em) {
    quadTree->clear();
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;
    start = std::chrono::system_clock::now();
    for(auto e : em.iterate<fmc::CCollider, fmc::CTransform>()) {
        fmc::CCollider* collider = e->get<fmc::CCollider>();
        fmc::CTransform* transform = e->get<fmc::CTransform>();

        fm::Object object;
        object.idObject = e->ID;
        object.rect = fm::Recti(transform->position.x, transform->position.y, collider->rect.w, collider->rect.h);
        quadTree->insert(object);
    }
    end = std::chrono::system_clock::now();
    elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}
void ColliderSystem::over() {
}