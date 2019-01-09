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

	virtual void Move(const XMFLOAT3 & Shift) override;
	virtual void Rotate(float x, float y, float z) override;

};

