#include "stdafx.h"
#include "MonsterMovement.h"
#include "DeadAction.h"

void DeadAction::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	movement->m_xmf3Velocity = XMFLOAT3(0.f, 0.f, 0.f);
}

void DeadAction::UpdateState(float fElpasedTime, MonsterActionMgr * actionMgr)
{
	// 죽은 이후에는 상태를 업데이트할 필요가 없습니다.
}
