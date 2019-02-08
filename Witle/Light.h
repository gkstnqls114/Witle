#pragma once
#include "MyStructs.h"
#include "ComponentBase.h"

struct LIGHT;

class Light
	: public ResourceComponentBase
{

protected:
	LIGHT m_Light;

public:
	Light(GameObject* pOwner);
	virtual ~Light();

	virtual void Update(float fTimeElapsed) = 0;

};