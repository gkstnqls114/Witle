#pragma once
#include "Camera.h"

// target�� �ڿ��� ����ٴϴ� ī�޶��Դϴ�.
class FollowCam :
	public Camera
{
	const GameObject		*m_pTarget{ nullptr };

private:

public:
	FollowCam(GameObject* target); // target�� �����մϴ�.
	FollowCam(Camera* camera, GameObject* target); // �θ� camera�� �״�� ����� target�� �����մϴ�.
	FollowCam(FollowCam* followcam); // followcam�� �״�� �����ɴϴ�.
	virtual ~FollowCam();

	void SetTarget(GameObject* pLookingObject) { m_pTarget = pLookingObject; }

	virtual void Move(const XMFLOAT3 & Shift) override;
	virtual void Rotate(float x, float y, float z) override;

	virtual void ZoomIn(float val);
	virtual void ZoomOut(float val);

};

