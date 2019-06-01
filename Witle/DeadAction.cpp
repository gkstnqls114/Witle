#include "stdafx.h"
#include "MonsterMovement.h"
#include "DeadAction.h"

void DeadAction::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	movement->m_xmf3Velocity = XMFLOAT3(0.f, 0.f, 0.f);
}
