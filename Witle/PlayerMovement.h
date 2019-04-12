#pragma once
#include "ComponentBase.h"

// �÷��̾ �̵��� �ʿ��� ���ӵ�, ȸ������ ���� ����
class PlayerMovement
	: public ComponentBase
{
public: 
	XMFLOAT3 m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float m_fMaxVelocityY{ 400.0f };
	float m_fMaxVelocityXZ{ 2000.0f };
	float m_fFriction = 1700.0f;
	float m_fDistance = 2000.f; // 20m / ��

	// ȸ����
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