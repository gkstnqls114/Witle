#include "stdafx.h"
#include "GameObject.h"
#include "MonsterActionMgr.h"

//
//XMFLOAT3 MonsterAction::AlreadyUpdate(float fTimeElapsed)
//{
//	if (Vector3::Length(m_xmf3Velocity) <= 0.f) return XMFLOAT3(0.0f, 0.0f, 0.0f); // �������� �ʴ� ������ ��� �׳� �Ѿ��.
//
//	XMFLOAT3 AlreadyVelocity = Vector3::Add(m_xmf3Velocity, m_xmf3Gravity);
//	float fLength = sqrtf(AlreadyVelocity.x * AlreadyVelocity.x + AlreadyVelocity.z * AlreadyVelocity.z);
//	float fMaxVelocityXZ = m_fMaxVelocityXZ;
//	if (fLength > m_fMaxVelocityXZ)
//	{
//		AlreadyVelocity.x *= (fMaxVelocityXZ / fLength);
//		AlreadyVelocity.z *= (fMaxVelocityXZ / fLength);
//	}
//	float fMaxVelocityY = m_fMaxVelocityY;
//	fLength = sqrtf(AlreadyVelocity.y * AlreadyVelocity.y);
//	if (fLength > m_fMaxVelocityY) AlreadyVelocity.y *= (fMaxVelocityY / fLength);
//
//	// �� �����ӿ� �ش��Ͽ� �̵�..
//	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(AlreadyVelocity, fTimeElapsed, false);
//
//	return xmf3Velocity;
//}
//
//void MonsterAction::MoveVelocity(DWORD dwDirection, float fTimeElapsed)
//{
//	AXIS axis = AXIS{ m_pOwner->GetTransform().GetCoorAxis() };
//
//	XMFLOAT3 xmf3Shift = XMFLOAT3(0.f, 0.f, 0.f); // �̵���
//
//	// �÷��̾ dwDirection �������� �̵��Ѵ�(�����δ� �ӵ� ���͸� �����Ѵ�).
//	// �̵� �Ÿ��� �ð��� ����ϵ��� �Ѵ�. �÷��̾��� �̵� �ӷ��� (20m/��)�� �����Ѵ�.
//	float fDistance = m_fDistance * fTimeElapsed; // 1�ʴ� �ִ� �ӷ� 20m���� ����, ���� 1 = 1cm
//
//	if (m_isBroomMode) // ���� ���ڷ� ����� ���...
//	{
//		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, axis.look, fDistance);
//		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, axis.look, -fDistance);
//		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, axis.right, fDistance);
//		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, axis.right, -fDistance);
//
//		MoveVelocity(xmf3Shift);
//	}
//	else
//	{
//		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, axis.look, m_fMaxVelocityXZ - 500);
//		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, axis.look, -m_fMaxVelocityXZ + 500);
//		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, axis.right, m_fMaxVelocityXZ - 500);
//		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, axis.right, -m_fMaxVelocityXZ + 500);
//
//		m_xmf3Velocity = xmf3Shift;
//	} 
//}
//
//void MonsterAction::ReduceVelocity(float fTimeElapsed)
//{
//	if (Vector3::Length(m_xmf3Velocity) <= 0.f)
//	{
//		m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
//		return; // �������� �ʴ� ������ ��� �׳� �Ѿ��.
//	}
//
//	MoveVelocity(Vector3::ScalarProduct(m_xmf3Velocity, -0.2f, false));
//
//}
