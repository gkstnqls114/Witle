#include "stdafx.h"
#include "Monster.h"
#include "Object.h" 
#include "GeneralMonsterActionMgr.h"
#include "MonsterMovement.h"
#include "HitAction.h"
#include "SoundManager.h"

void HitAction::UpdateVelocity(float fElpasedTime, Movement * movement)
{ 
	movement->SetVelocity( XMFLOAT3(0.f, 0.f, 0.f));
}

void HitAction::UpdateState(float fElpasedTime, GeneralMonsterActionMgr * actionMgr)
{ 
	// ���� ������ ��Ʈ �׼��� �����ٸ�...
	if (static_cast<Monster*>(m_pOwner)->GetpLoadObject()->IsTrackAnimationSetFinish(0, SPACECAT_HIT.ID))
	{
		actionMgr->ChangeStateBefore();
	} 
}
