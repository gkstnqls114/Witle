#pragma once
#include "Camera.h"
class FollowCam :
	public Camera
{
	const GameObject		*m_pTarget{ nullptr };

private:
	void RotateOnLookingobject(float x, float y, float z);
	void MoveAroundLookingobject(const XMFLOAT3& Shift);

public:
	FollowCam(GameObject* target); // target을 지정합니다.
	FollowCam(Camera* camera, GameObject* target); // 부모 camera를 그대로 갖고와 target을 지정합니다.
	FollowCam(FollowCam* followcam); // followcam을 그대로 가져옵니다.
	virtual ~FollowCam();

	void SetTarget(GameObject* pLookingObject) { m_pTarget = pLookingObject; }

	virtual void Move(const XMFLOAT3 & Shift) override;
	virtual void Rotate(float x, float y, float z) override;

	virtual void ZoomIn(float val);
	virtual void ZoomOut(float val);

};

