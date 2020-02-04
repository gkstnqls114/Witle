#include "stdafx.h"
#include "MonsterMovement.h"
#include "DeadAction.h"

#include "SoundManager.h"

void DeadAction::UpdateVelocity(float fElpasedTime, Movement * movement)
{ 
	movement->SetVelocity(XMFLOAT3(0.f, 0.f, 0.f));
}

void DeadAction::UpdateState(float fElpasedTime, GeneralMonsterActionMgr * actionMgr)
{
	// ���� ���Ŀ��� ���¸� ������Ʈ�� �ʿ䰡 �����ϴ�.
}
