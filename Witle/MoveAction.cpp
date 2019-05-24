#include "stdafx.h"
#include "MonsterMovement.h"
#include "MoveAction.h"

void MoveAction::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{ 
	movement->m_xmf3Velocity = m_Direction;
}
