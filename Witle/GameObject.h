#pragma once
#include "ID.h"

class ComponentBase;
class GameObject
{
public:
	GameObject(EntityID entityID);
	virtual ~GameObject();

	void Update() {};
	
	bool InsertComponent(ComponentBase* pComponentBase);
	ComponentBase* GetComponent(const ComponenetID& id);
	const ComponentBase* GetComponent(const ComponenetID& id) const;

private:
	std::map<ComponenetID, ComponentBase*> m_Components;
};

