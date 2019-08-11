#include "stdafx.h"
#include "GameScene.h"
#include "MonsterMovement.h"
#include "BossChaseAction.h"
#include "Monster.h"
#include "MonsterStatus.h"
#include "MyBOBox.h"
#include "Player.h"
#include "Dragon.h"
#include "BossMonsterActionMgr.h"
#include "PlayerManager.h"
#include "Collision.h"


ENUM_BOSSSKILL BossChaseAction::GetRandomSkill()
{
	ENUM_BOSSSKILL result;

	float hp = static_cast<Monster*>(m_pOwner)->GetStatus()->m_Guage;

	std::random_device rd;
	std::mt19937 mersenne(rd());
	std::uniform_int_distribution<> die(2000, 15000);

	if (hp > 70.f) // first phase 
	{
		std::uniform_int_distribution<> monstertype(2, 2);
		/*
		BOSSSKILL_BREATH,
		BOSSSKILL_DOWNSTROKE 중 하나
		*/
		result = ENUM_BOSSSKILL(monstertype(mersenne)); // 0 혹은 1
	}
	else if (hp > 30.f)  // second phase
	{
		std::uniform_int_distribution<> monstertype(0, 2);
		/*
		BOSSSKILL_BREATH,
		BOSSSKILL_DOWNSTROKE  
		BOSSSKILL_TAILATTACK
		*/
		result = ENUM_BOSSSKILL(monstertype(mersenne)); // 0 혹은 1 
	}
	else  // last phase
	{
		std::uniform_int_distribution<> monstertype(0, 3);
		/*
		BOSSSKILL_BREATH,
		BOSSSKILL_DOWNSTROKE 
		BOSSSKILL_TAILATTACK
		BOSSSKILL_RUSH
		*/
		result = ENUM_BOSSSKILL(monstertype(mersenne)); // 0 혹은 1
	}

	return result;
}

float BossChaseAction::GetDistance(ENUM_BOSSSKILL skill)
{
	float distance = 0.f;

	switch (skill)
	{
	case BOSSSKILL_NONE:
		break;
	case BOSSSKILL_BREATH:
		distance = m_fBreathDistance;
		break;
	case BOSSSKILL_DOWNSTROKE:
		distance = m_fDownStrokeDistance;
		break;
	case BOSSSKILL_TAILATTACK:
		distance = m_fTailAttackDistance;
		break;
	case BOSSSKILL_RUSH:
		distance = m_fRushDistance;
		break;
	default:
		break;
	}

	return distance;
}
 
void BossChaseAction::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	XMFLOAT3 toPlayer = Vector3::Normalize(
		Vector3::Subtract(PlayerManager::GetMainPlayer()->GetTransform().GetPosition(), m_pOwner->GetTransform().GetPosition())
	);

	movement->m_xmf3Velocity = Vector3::ScalarProduct(toPlayer, movement->m_fDistance, false);

	XMFLOAT3 look(0.f, 0.f, 1.f);
	float value = Vector3::AngleToRadian(look, toPlayer) * 180.f / 3.141592;
	if (Vector3::CrossProduct(look, toPlayer).y >= 0)
	{
		movement->m_fPitch = value;
	}
	else
	{
		movement->m_fPitch = -value;
	}
}

void BossChaseAction::UpdateState(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{
	// 만약 체력이 0라면 상태를 
	if (static_cast<Monster*>(m_pOwner)->GetStatus()->m_Guage <= 0.f)
	{
		(actionMgr)->ChangeBossStateToDead(); 
		return;
	}
	  
	ENUM_BOSSSKILL skill = GetRandomSkill(); 
	// 플레이어 스킬에 따른 이동 거리를 가져옵니다. 
	// 만약 플레이어와 해당 거리 이하면 상태를 전환하지 않습니다.
	if (!PlayerManager::IsNearPlayer(m_pOwner->GetTransform().GetPosition(), GetDistance(skill))) return;

	switch (skill)
	{
	case BOSSSKILL_NONE:
		break;
	case BOSSSKILL_BREATH:
		(actionMgr)->ChangeBossStateToBreath();
		break;
	case BOSSSKILL_DOWNSTROKE:
		(actionMgr)->ChangeBossStateToDownStroke();
		break;
	case BOSSSKILL_TAILATTACK:
		(actionMgr)->ChangeBossStateToTailAttack();
		break;
	case BOSSSKILL_RUSH: 
		(actionMgr)->ChangeBossStateToRush();
		break;
	default:
		break;
	} 
}