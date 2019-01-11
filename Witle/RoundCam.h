#pragma once
#include "Camera.h"

// target�� �������� �ѷ� ���� ī�޶��Դϴ�.
class RoundCam :
	public Camera
{
	const GameObject		*m_pTarget{ nullptr };

private:
	
public:
	RoundCam(GameObject* target); // target�� �����մϴ�.
	RoundCam(Camera* camera, GameObject* target); // �θ� camera�� �״�� ����� target�� �����մϴ�.
	RoundCam(RoundCam* followcam); // followcam�� �״�� �����ɴϴ�.
	virtual ~RoundCam();

	void SetTarget(GameObject* pLookingObject) { m_pTarget = pLookingObject; }

	virtual void Move(const XMFLOAT3 & Shift) override;
	virtual void Rotate(float x, float y, float z) override;

	virtual void ZoomIn(float val);
	virtual void ZoomOut(float val);
};

