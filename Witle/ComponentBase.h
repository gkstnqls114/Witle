#pragma once
#include "ID.h"

class GameObject;
class ComponentBase
{
public:
	ComponentBase(GameObject* pOwner) : m_pOwner(pOwner) {}
	virtual ~ComponentBase();
	 
	int GetComponentID() const { return m_ComponenetID; };
	const std::string& GetFamillyID() const { return m_FamillyID; };

	virtual void Update(float fTimeElapsed) = 0;
	
	void SetOwner(GameObject* pOwner) { m_pOwner = pOwner; }
	GameObject* GetOwmer() const { return m_pOwner; };

private:
	GameObject* m_pOwner{ nullptr };

protected: 
	UINT m_ComponenetID{ 0 };
	std::string m_FamillyID{ "ComponentBase" };
};

