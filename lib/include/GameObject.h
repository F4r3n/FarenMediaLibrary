#pragma once

namespace fm {
class GameObject {
public:
	GameObject();
	virtual ~GameObject();
	virtual void update(float dt) = 0;
	virtual void init() = 0;
private:


};
}