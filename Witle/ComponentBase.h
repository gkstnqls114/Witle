#pragma once
#include "FamilyID.h"

class GameObject;
class ResourceBase;

class ComponentBase
{
public:
	ComponentBase(GameObject* pOwner) : m_pOwner(pOwner) {}
	virtual ~ComponentBase();
	 
	virtual void Update(float fTimeElapsed) = 0;
	 
	int GetComponentID() const { return m_ComponenetID; };
	const FamilyID& GetFamilyID() const { return m_FamilyID; };

	
protected: 
	GameObject* const m_pOwner{ nullptr };
	
	UINT m_ComponenetID{ 0 };
	FamilyID m_FamilyID;
};

class ResourceComponentBase
	: public ComponentBase
{
protected:
	ResourceBase	*m_ShaderVariables{ nullptr };

public:
	ResourceComponentBase(GameObject* pOwner) : ComponentBase(pOwner) {}
	virtual ~ResourceComponentBase();

	const ResourceBase* const GetResource() const { return m_ShaderVariables; }
};