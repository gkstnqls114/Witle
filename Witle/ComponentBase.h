#pragma once
#include "ID.h"

class GameObject;
class ComponentBase
{
public:
	ComponentBase() ;
	virtual ~ComponentBase();

	virtual const ComponenetID& GetComponentID() const = 0;
	virtual const ComponenetID& GetFamillyID() const = 0;

	virtual void Update() = 0;
	
	void SetOwner(GameObject* pOwner) { m_pOwner = pOwner; }
	GameObject* GetOwmer() const { return m_pOwner; };


private:
	GameObject* m_pOwner{ nullptr };
	
};

