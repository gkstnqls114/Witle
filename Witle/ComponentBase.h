#pragma once
#include "ID.h"

class GameObject;
class ComponentBase
{
protected:
	// ComponentBase(GameObject* const pOwner) : m_pOwner(pOwner) {}

public:
	ComponentBase(GameObject* pOwner) : m_pOwner(pOwner) {}
	virtual ~ComponentBase();
	 
	int GetComponentID() const { return m_ComponenetID; };
	const std::string& GetFamillyID() const { return m_FamillyID; };

	virtual void Update(float fTimeElapsed) = 0;
	
	// void SetOwner(GameObject* pOwner) { m_pOwner = pOwner; }
	// GameObject* const GetOwner() const { return m_pOwner; };

protected: 
	GameObject* const m_pOwner{ nullptr };
	
	UINT m_ComponenetID{ 0 };
	std::string m_FamillyID{ "ComponentBase" };
};

