#include "stdafx.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Monster.h"
#include "RecognitionRange.h"
#include "MonsterMovement.h"
#include "BossMonsterActionMgr.h"
#include "BossSampleAction_1.h"

void BossSampleAction_1::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	// �������� �ʵ��� ����
	movement->m_xmf3Velocity = XMFLOAT3(0.f, 0.f, 0.f);
}

void BossSampleAction_1::UpdateState(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{
	Monster* pMonsterOwner = static_cast<Monster*>(m_pOwner);

	bool isNearPlayer = PlayerManager::IsNearPlayer(
		m_pOwner->GetTransform().GetPosition(),
		500
	);

	if (isNearPlayer)
	{  
		actionMgr->ChangeStateToSample_2(); 
	} 
}
