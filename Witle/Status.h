#pragma once
#include "ComponentBase.h"

class Shader;
class MyRectangle;

// HP 관련 정보
class Status
	: public ComponentBase
{ 
protected:
	virtual void SetAnimationStateToHit() = 0;
	void SubstractHP(int damage);

public:
	int m_HP{ 1000 };
	int m_MP{ 1000 };

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};
	  
public:
	Status(GameObject* pOwner );
	virtual ~Status(); 
	virtual void Damage(int damage, int state) = 0;
};