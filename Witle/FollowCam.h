#pragma once
#include "Camera.h"

// target을 뒤에서 따라다니는 카메라입니다.
class FollowCam :
	public Camera
{
	const GameObject		*m_pTarget{ nullptr };

private:
	virtual void MoveSmoothly(float fTimeElapsed, const XMFLOAT3& xmf3LookAt) override;

	void SetLookAt(const XMFLOAT3 & xmf3LookAt);


public:
	FollowCam(GameObject* target); // target을 지정합니다.
	FollowCam(Camera* camera, GameObject* target); // 부모 camera를 그대로 갖고와 target을 지정합니다.
	FollowCam(FollowCam* followcam); // followcam을 그대로 가져옵니다.
	virtual ~FollowCam();

	void SetTarget(GameObject* pLookingObject) { m_pTarget = pLookingObject; }
	virtual void Update(float fTimeElapsed, const XMFLOAT3& xmf3LookAt);


	virtual void Rotate(float x, float y, float z) override;

	virtual void ZoomIn(float val);
	virtual void ZoomOut(float val);

};

