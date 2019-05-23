#pragma once
#include "ComponentBase.h"

// �÷��̾ �̵��� �ʿ��� ���ӵ�, ȸ������ ���� ����
class MonsterMovement
	: public ComponentBase
{
public:
	bool m_isBroomMode{ false };

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

	const float m_fNearDistance = 500.f;

private:
	bool IsNearPlayer(const GameObject* Target);

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	MonsterMovement(GameObject* pOwner);
	virtual ~MonsterMovement();

	// �̵��� ���õ� ȣ��
	virtual void Update(float) override;

	// Update ���� ���� �ݵ�� ȣ��
	virtual void ProcessAI(float fElpasedTime, const XMFLOAT3& randomDirection, const GameObject* pTarget);

	XMFLOAT3 AlreadyUpdate(float);

	void MoveVelocity(DWORD dwDirection, float);
	void MoveVelocity(const XMFLOAT3& shift);

	void ReduceVelocity(float);

	void BroomMode();
	void RunMode();
};