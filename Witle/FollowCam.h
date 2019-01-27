#pragma once
#include "Camera.h"

// target�� �ڿ��� ����ٴϴ� ī�޶��Դϴ�.
class FollowCam :
	public Camera
{
	const GameObject		*m_pTarget{ nullptr };

	void* m_pCameraUpdatedContext{ nullptr };

private:
	virtual void MoveSmoothly(float fTimeElapsed, const XMFLOAT3& xmf3LookAt) override;
	void OnCameraUpdateCallback(float fTimeElapsed);

public:
	FollowCam(GameObject* pOwner, GameObject* target); // target�� �����մϴ�.
	FollowCam(GameObject* pOwner, Camera* camera, GameObject* target); // �θ� camera�� �״�� ����� target�� �����մϴ�.
	FollowCam(FollowCam* followcam); // followcam�� �״�� �����ɴϴ�.
	virtual ~FollowCam();

	void SetTarget(GameObject* pLookingObject) { m_pTarget = pLookingObject; }

	virtual void Teleport(const XMFLOAT3& at) override; // right, up, look�� ������ ���·� position, at�� �̵��Ѵ�.
	virtual void Update(float fTimeElapsed, const XMFLOAT3& xmf3LookAt) override;
	virtual void LastUpdate(float fTimeElapsed) override;

	void SetLookAt(const XMFLOAT3 & xmf3LookAt);
	virtual void Rotate(float x, float y, float z) override;

	virtual void ZoomIn(float val);
	virtual void ZoomOut(float val);

};

