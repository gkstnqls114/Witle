#pragma once
#include "Camera.h"
class FollowCam :
	public Camera
{
	GameObject		*m_pTarget{ nullptr };

public:
	FollowCam();
	virtual ~FollowCam();

	void SetLookingObject(GameObject* pLookingObject) { m_pTarget = pLookingObject; }

};

