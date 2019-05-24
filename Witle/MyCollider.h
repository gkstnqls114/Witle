#pragma once
#include "ComponentBase.h"
 
class MyCollider
	: public ComponentBase
{
public:
	virtual void Update(float fTimeElapsed) override {};
	virtual void ReleaseObjects() override = 0;
	virtual void ReleaseUploadBuffers() override = 0;

public:
	MyCollider(GameObject* pOwner, COLLIDER_TYPE type);
	virtual ~MyCollider();

	COLLIDER_TYPE GetType() const { return m_ColliderType; }
	 
private:
	COLLIDER_TYPE m_ColliderType;
}; 