#include "stdafx.h"
#include "MonsterMovement.h"
#include "MoveAction.h"

void MoveAction::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{ 
	movement->m_xmf3Velocity = Vector3::ScalarProduct(m_Direction, movement->m_fDistance, false);
}
