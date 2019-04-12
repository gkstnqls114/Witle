#pragma once
#include "ComponentBase.h"

// 플레이어에 이동에 필요한 가속도, 회전량에 대한 설정
class PlayerMovement
	: public ComponentBase
{
public: 
	XMFLOAT3 m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float m_fMaxVelocityY{ 400.0f };
	float m_fMaxVelocityXZ{ 2000.0f };
	float m_fFriction = 1700.0f;
	float m_fDistance = 2000.f; // 20m / 초

	// 회전량
	float m_fPitch = 0.0f;
	float m_fRoll = 0.0f;
	float m_fYaw = 0.0f;

public:
	PlayerMovement(GameObject* pOwner);
	virtual ~PlayerMovement();

	virtual void Update(float) override;

	void BroomMode();
	void RunMode();
};