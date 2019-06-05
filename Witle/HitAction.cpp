#include "stdafx.h"
#include "MonsterMovement.h"
#include "HitAction.h"

void HitAction::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	movement->m_xmf3Velocity = XMFLOAT3(0.f, 0.f, 0.f);
}

void HitAction::UpdateState(float fElpasedTime, MonsterActionMgr * actionMgr)
{
}
