#pragma once
#include "ComponentBase.h"
 
class MyCollider
	: public ComponentBase
{
public:
	MyCollider(GameObject* pOwner, COLLIDER_TYPE type);
	virtual ~MyCollider();

private:
	COLLIDER_TYPE m_ColliderType;

public:
	virtual void Update(float fTimeElapsed) override {};
	virtual void ReleaseObjects() override = 0;
	virtual void ReleaseUploadBuffers() override = 0;

	COLLIDER_TYPE GetType() const { return m_ColliderType; }
}; 