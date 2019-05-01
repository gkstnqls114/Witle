#pragma once
#include "FamilyID.h"

class GameObject;
class ResourceBase;

class IComponentBase
{
public:
	virtual void Update(float fTimeElapsed) = 0;
	virtual void ReleaseObjects() = 0;
	virtual void ReleaseUploadBuffers() = 0;
};

class ComponentBase
	: public IComponentBase
{
protected:
	ComponentBase() {}

public:
	ComponentBase(GameObject* pOwner) : m_pOwner(pOwner) {}
	virtual ~ComponentBase();
	  
	int GetComponentID() const { return m_ComponenetID; };
	const FamilyID& GetFamilyID() const { return m_FamilyID; };

	virtual void Update(float fTimeElapsed) override {};
	virtual void ReleaseObjects() = 0;
	virtual void ReleaseUploadBuffers() = 0;
	
protected: 
	GameObject* const m_pOwner{ nullptr };
	
	UINT m_ComponenetID{ 0 };
	FamilyID m_FamilyID;
};
