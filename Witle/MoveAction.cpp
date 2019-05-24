#include "stdafx.h"
#include "MonsterMovement.h"
#include "MoveAction.h"

void MoveAction::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	// movement->m_xmf3Velocity = randomDirection;
	movement->m_xmf3Velocity = XMFLOAT3(10.f, 0.f, 10.f);
}
