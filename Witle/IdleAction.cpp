#include "stdafx.h"
#include "MonsterMovement.h"
#include "IdleAction.h"

void IdleAction::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	// �������� �ʵ��� ����
	movement->m_xmf3Velocity = XMFLOAT3(0.f, 0.f, 0.f);
}
