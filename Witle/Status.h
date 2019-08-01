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

public:
	int m_Guage{ 100 };
	int m_MAXGuage{ 100 };
	
public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};
	  
public:
	Status(GameObject* pOwner );
	virtual ~Status(); 
	virtual void Damage(int damage, int state) = 0;

	void SubstractHP(UINT damage);
	void AddHP(UINT damage);
};