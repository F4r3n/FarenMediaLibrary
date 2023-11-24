#pragma once
#include "Core/System/System.hpp"
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

    void update(float dt, entt::registry& em, EventManager& event) override;
    void init(EventManager& event) override;
	void Start(entt::registry&) override {}
	void Stop(entt::registry&) override {}
private:
    void _SetSettings(const fm::Transform& transform, fmc::CSource* sound);
	std::unique_ptr<fm::Speaker> _speaker;
	std::unique_ptr<fm::Listener> _listener;
};
}