#pragma once
#include "Camera.h"

class BasicCam
	:public Camera
{

public:
	BasicCam(GameObject* pOwner);
	virtual ~BasicCam();

	virtual void LastUpdate(float fTimeElapsed) override;

};