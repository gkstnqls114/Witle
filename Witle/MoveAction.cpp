#include "stdafx.h"
#include "MonsterMovement.h"
#include "MoveAction.h"

void MoveAction::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{ 
	movement->m_xmf3Velocity = Vector3::ScalarProduct(m_Direction, movement->m_fDistance, false);
	
	XMFLOAT3 look (0.f, 0.f, 1.f);
	float value = Vector3::AngleToRadian(look, m_Direction) * 180.f / 3.141592;
	if (Vector3::CrossProduct(look, m_Direction).y >= 0)
	{ 
		movement->m_fPitch = value;
	}
	else
	{
		movement->m_fPitch = -value;
	} 

}
