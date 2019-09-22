#pragma once
#include "ComponentBase.h"

class Shader;
class MyRectangle;

// HP 관련 정보
class Status
	: public ComponentBase
{ 
protected:
	virtual void ChangeAfterActionToHit() = 0;
	 
	float m_Guage{ 100.f };
	
public:
	float m_MAXGuage{ 100.f };
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};
	  
public:
	Status(GameObject* pOwner );
	virtual ~Status(); 
	virtual void Damage(int damage, int state) = 0;

	float GetGuage() { return m_Guage; }
	void SetGuage(float g) { m_Guage = g; }

	virtual void SubstractHP(float damage);
	void AddHP(float damage);
};