#pragma once
#include "ComponentBase.h"

// 플레이어에 이동에 필요한 가속도, 회전량에 대한 설정
// 이동에 필요한 가속도, 중력가속도, 최대
class Movement
	: public ComponentBase
{
public:  
	XMFLOAT3 m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f); // 가속도
	XMFLOAT3 m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);  // 중력가속도
	float m_fMaxVelocityY{ 400.0f };					  // 최대 y축 가속도
	float m_fMaxVelocityXZ{ 2000.0f };					  // 최대 x축과 z축 가속도
	float m_fFriction = 1700.0f;						  // 마찰력
	float m_fDistance = 2000.f;							  // 초당 m (2000 = 20m)

	float m_fPitch = 0.0f;								  // y축 회전
	float m_fRoll = 0.0f;								  // x축 회전
	float m_fYaw = 0.0f;							      // z축 회전

protected:
	// shift 만큼 가속도를 증감합니다.
	void MoveVelocity(const XMFLOAT3& shift);

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	Movement(GameObject* pOwner);
	virtual ~Movement();

	virtual void Update(float) override;

	// 해당 프레임의 가속도를 계산한 값을 리턴합니다.
	XMFLOAT3 AlreadyUpdate(float);
};