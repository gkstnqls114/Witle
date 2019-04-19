#pragma once
#include "Light.h"

class DirectionalLight
	: public Light
{

public:
	DirectionalLight(GameObject * pOwner);
	virtual ~DirectionalLight();

	virtual void Update(float fTimeElapsed) override;

};