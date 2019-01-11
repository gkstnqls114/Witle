#pragma once
#include "Camera.h"

// target을 주위에서 둘러 보는 카메라입니다.
class RoundCam :
	public Camera
{
	const GameObject		*m_pTarget{ nullptr };

private:
	void RotateOnLookingobject(float x, float y, float z);
	void MoveAroundLookingobject(const XMFLOAT3& Shift);

public:
	RoundCam(GameObject* target); // target을 지정합니다.
	RoundCam(Camera* camera, GameObject* target); // 부모 camera를 그대로 갖고와 target을 지정합니다.
	RoundCam(RoundCam* followcam); // followcam을 그대로 가져옵니다.
	virtual ~RoundCam();

	void SetTarget(GameObject* pLookingObject) { m_pTarget = pLookingObject; }

	virtual void Move(const XMFLOAT3 & Shift) override;
	virtual void Rotate(float x, float y, float z) override;

	virtual void ZoomIn(float val);
	virtual void ZoomOut(float val);
};

