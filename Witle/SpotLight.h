#pragma once 
#include "Light.h"

class SpotLight
	: public Light
{
private:

public:
	SpotLight(GameObject * pOwner);
	virtual ~SpotLight();

	virtual void Update(float fTimeElapsed) override;

};