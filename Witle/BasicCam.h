#pragma once
#include "Camera.h"

class BasicCam
	:public Camera
{

public:
	BasicCam(GameObject* pOwner);
	virtual ~BasicCam();

	virtual void LastUpdate(float fTimeElapsed) override;
	virtual void Rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f) override;

};