#pragma once
#include "System.h"
#include <EntityManager.h>
#include <memory>
namespace fmc {
class CTransform;
class CSource;
}


namespace fm
{
	class Speaker;
	class Listener;
	struct Transform;
}

namespace fms {
class SoundSystem : public System<SoundSystem> {
public:
    SoundSystem();
    ~SoundSystem();
    void pre_update(EntityManager& em);

    void update(float dt, EntityManager& em, EventManager& event);
    void init(EntityManager& em, EventManager& event);
    void over();
	void Start() {}
	void Stop() {}
private:
    void _SetSettings(const fm::Transform& transform, fmc::CSource* sound);
	std::unique_ptr<fm::Speaker> _speaker;
	std::unique_ptr<fm::Listener> _listener;
};
}