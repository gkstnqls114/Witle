#include "stdafx.h"
#include "GameObject.h"
#include "MonsterActionMgr.h"

//
//XMFLOAT3 MonsterAction::AlreadyUpdate(float fTimeElapsed)
//{
//	if (Vector3::Length(m_xmf3Velocity) <= 0.f) return XMFLOAT3(0.0f, 0.0f, 0.0f); // 움직이지 않는 상태일 경우 그냥 넘어간다.
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
//	// 한 프레임에 해당하여 이동..
//	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(AlreadyVelocity, fTimeElapsed, false);
//
//	return xmf3Velocity;
//}
//
//void MonsterAction::MoveVelocity(DWORD dwDirection, float fTimeElapsed)
//{
//	AXIS axis = AXIS{ m_pOwner->GetTransform().GetCoorAxis() };
//
//	XMFLOAT3 xmf3Shift = XMFLOAT3(0.f, 0.f, 0.f); // 이동량
//
//	// 플레이어를 dwDirection 방향으로 이동한다(실제로는 속도 벡터를 변경한다).
//	// 이동 거리는 시간에 비례하도록 한다. 플레이어의 이동 속력은 (20m/초)로 가정한다.
//	float fDistance = m_fDistance * fTimeElapsed; // 1초당 최대 속력 20m으로 가정, 현재 1 = 1cm
//
//	if (m_isBroomMode) // 만약 빗자루 모드일 경우...
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
//		return; // 움직이지 않는 상태일 경우 그냥 넘어간다.
//	}
//
//	MoveVelocity(Vector3::ScalarProduct(m_xmf3Velocity, -0.2f, false));
//
//}
