#pragma once

namespace fmc
{
	class CTransform;
	class CMaterial;
	class Body2D;
	class CDirectionalLight;
	class CMesh;
	class CText;
}

namespace fm
{
	class GameObject;
}


class Inspector
{
public:
	virtual ~Inspector() { ; }
	virtual void Draw(bool*, std::shared_ptr<fm::GameObject> inGameObject) = 0;
	virtual void RemoveComponent(std::shared_ptr<fm::GameObject> inGameObject) = 0;

};
