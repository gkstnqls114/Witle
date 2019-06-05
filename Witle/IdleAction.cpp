#include "stdafx.h"
#include "MonsterMovement.h"
#include "IdleAction.h"

void IdleAction::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	// 움직이지 않도록 고정
	movement->m_xmf3Velocity = XMFLOAT3(0.f, 0.f, 0.f);
}

void IdleAction::UpdateState(float fElpasedTime, MonsterActionMgr * actionMgr)
{
}
