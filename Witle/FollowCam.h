#pragma once
#include "Camera.h"

class LoadObject;
// target을 뒤에서 따라다니는 카메라입니다.
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
	FollowCam(GameObject* pOwner, GameObject* target); // target을 지정합니다.
	FollowCam(GameObject* pOwner, Camera* camera, GameObject* target); // 부모 camera를 그대로 갖고와 target을 지정합니다.
	FollowCam(GameObject* pOwner, FollowCam* followcam); // followcam을 그대로 가져옵니다.
	virtual ~FollowCam();

	void SetTarget(GameObject* pLookingObject) { m_pTarget = pLookingObject; }

	virtual void Teleport(const XMFLOAT3& at) override; // right, up, look을 유지한 상태로 position, at만 이동한다.
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
