#pragma once
#include "ID.h"

class GameObject;
class ComponentBase
{
public:
	ComponentBase();
	virtual ~ComponentBase();
	 
	const std::string& GetComponentID() const { return m_ComponenetID; };
	const std::string& GetFamillyID() const { return m_FamillyID; };

	virtual void Update() = 0;
	
	void SetOwner(GameObject* pOwner) { m_pOwner = pOwner; }
	GameObject* GetOwmer() const { return m_pOwner; };

private:
	GameObject* m_pOwner{ nullptr };

protected: 
	std::string m_ComponenetID{ "ComponentBase" };
	std::string m_FamillyID{ "ComponentBase" };
};

