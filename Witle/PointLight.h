#pragma once
#include "Light.h"

class PointLight
	: public Light
{
private:

public:
	PointLight(GameObject * pOwner);
	virtual ~PointLight();
	
	virtual void Update(float fTimeElapsed) override;

};