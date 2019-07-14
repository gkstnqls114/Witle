#include "stdafx.h"

#include "Object.h"
#include "PlayerManager.h"
#include "Player.h"
#include "RecognitionRange.h"
#include "MonsterStatus.h"
#include "Monster.h"

#include "BossMonsterActionMgr.h"
    
void BossMonsterActionMgr::UpdateState(float fElpasedTime)
{ 
	static_cast<BossMonsterAction*>(m_CurrMonsterAction)->UpdateState(fElpasedTime, this);
}
 

void BossMonsterActionMgr::ChangeStateToSample_1()
{
	ChangeAction(&m_BossSampleAction_1);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_IDLE.ID); 
}

void BossMonsterActionMgr::ChangeStateToSample_2()
{
	ChangeAction(&m_BossSampleAction_2);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_MOVE.ID);
}

// void BossMonsterActionMgr::BossIdleAction()
// {
// 	ChangeAction(&m_BossSampleAction_2);
// 	m_CurrMonsterAction->Init();
// 	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_MOVE.ID);
// }
 