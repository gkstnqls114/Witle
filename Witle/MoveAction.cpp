#include "stdafx.h"
#include "MonsterMovement.h"
#include "MoveAction.h"

void MoveAction::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{ 
	movement->m_xmf3Velocity = XMFLOAT3(100.f, 0.f, 100.f);
}
