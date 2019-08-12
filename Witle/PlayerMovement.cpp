#include "stdafx.h"
#include "GameObject.h"
#include "PlayerMovement.h"

#include "SoundManager.h"

#define RUN_MAX_VELOCITY 950.F

void PlayerMovement::MoveVelocity(const XMFLOAT3 & xmf3Shift)
{
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
}

PlayerMovement::PlayerMovement(GameObject * pOwner)
	:Movement(pOwner)
{
	RunMode();
}

PlayerMovement::~PlayerMovement()
{

}

void PlayerMovement::MoveVelocity(DWORD dwDirection, float fTimeElapsed)
{
	AXIS axis = AXIS{ m_pOwner->GetTransform().GetCoorAxis() };

	XMFLOAT3 xmf3Shift = XMFLOAT3(0.f, 0.f, 0.f); // �̵���

	// �÷��̾ dwDirection �������� �̵��Ѵ�(�����δ� �ӵ� ���͸� �����Ѵ�).
	// �̵� �Ÿ��� �ð��� ����ϵ��� �Ѵ�. �÷��̾��� �̵� �ӷ��� (20m/��)�� �����Ѵ�.
	float fDistance = m_fDistance * fTimeElapsed; // 1�ʴ� �ִ� �ӷ� 20m���� ����, ���� 1 = 1cm

	if (m_isBroomMode) // ���� ���ڷ� ����� ���...
	{
		SoundManager::GetInstance()->Stop(ENUM_SOUND::PLAYER_MOVE);
		SoundManager::GetInstance()->Play(ENUM_SOUND::PLAYER_BROOM);

		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, axis.look, fDistance);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, axis.look, -fDistance);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, axis.right, fDistance);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, axis.right, -fDistance);

		MoveVelocity(xmf3Shift);

	}
	else
	{
		SoundManager::GetInstance()->Stop(ENUM_SOUND::PLAYER_BROOM);
		SoundManager::GetInstance()->Play(ENUM_SOUND::PLAYER_MOVE);

		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, axis.look, m_fMaxVelocityXZ - 500);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, axis.look, -m_fMaxVelocityXZ + 500);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, axis.right, m_fMaxVelocityXZ - 500);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, axis.right, -m_fMaxVelocityXZ + 500);

		m_xmf3Velocity = xmf3Shift;

	}

}

void PlayerMovement::ReduceVelocity(float fTimeElapsed)
{
	if (Vector3::Length(m_xmf3Velocity) <= 0.f)
	{
		m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
		return; // �������� �ʴ� ������ ��� �׳� �Ѿ��.
	}

	if (m_isBroomMode)
	{
		float fLength = Vector3::Length(m_xmf3Velocity);
		float fDeceleration = (m_fFriction * fTimeElapsed); //�ش����� Friction
		if (fDeceleration > fLength) fDeceleration = fLength;
		MoveVelocity(Vector3::ScalarProduct(m_xmf3Velocity, -fDeceleration, true));
	}
	else
	{
		MoveVelocity(Vector3::ScalarProduct(m_xmf3Velocity, -0.2f, false));
	}
}

void PlayerMovement::BroomMode()
{
	m_isBroomMode = true;
	m_fMaxVelocityXZ = 5000.0f;
	m_fFriction = 3000.0f;
	m_fDistance = 5000.f;
}

void PlayerMovement::RunMode()
{
	m_isBroomMode = false;
	m_fMaxVelocityXZ = RUN_MAX_VELOCITY;
	m_fFriction = 100000.0f;
	m_fDistance = 0.f;
}
