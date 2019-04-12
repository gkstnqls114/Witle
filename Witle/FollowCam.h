#pragma once
#include "Camera.h"

class LoadObject;
// target�� �ڿ��� ����ٴϴ� ī�޶��Դϴ�.
class FollowCam :
	public Camera
{
	const GameObject		*m_pTarget{ nullptr };
	XMFLOAT3				 m_distanceAt{ 0.f, 0.f, 0.f };

private:
	virtual void MoveSmoothly(float fTimeElapsed, const XMFLOAT3& xmf3LookAt) override;

protected:
	FollowCam(GameObject* pOwner) : Camera(pOwner) {}; //TEST

public:
	FollowCam(GameObject* pOwner, GameObject* target); // target�� �����մϴ�.
	FollowCam(GameObject* pOwner, Camera* camera, GameObject* target); // �θ� camera�� �״�� ����� target�� �����մϴ�.
	FollowCam(GameObject* pOwner, FollowCam* followcam); // followcam�� �״�� �����ɴϴ�.
	virtual ~FollowCam();

	void SetTarget(GameObject* pLookingObject) { m_pTarget = pLookingObject; }

	virtual void Teleport(const XMFLOAT3& at) override; // right, up, look�� ������ ���·� position, at�� �̵��Ѵ�.
	virtual void Update(float fTimeElapsed, const XMFLOAT3& xmf3LookAt) override;
	virtual void LastUpdate(float fTimeElapsed) override;

	void SetLookAt(const XMFLOAT3 & xmf3LookAt);
	void SetdistanceAt(const XMFLOAT3 & at) { m_distanceAt = at; }
	virtual void Rotate(float x, float y, float z) override;

	virtual void ZoomIn(float val);
	virtual void ZoomOut(float val);

};

class FollowCamForLoad
	: public FollowCam
{
	LoadObject		*m_pLoadTarget{ nullptr };

public:
	FollowCamForLoad(GameObject* pOwner, LoadObject* target);
	virtual ~FollowCamForLoad() {};
	virtual void LastUpdate(float fTimeElapsed) override;
	virtual void Rotate(float x, float y, float z) override;
};
