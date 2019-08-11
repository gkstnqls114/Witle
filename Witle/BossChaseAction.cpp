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

#include "SoundManager.h"

ENUM_BOSSSKILL BossChaseAction::GetRandomSkill()
{
	ENUM_BOSSSKILL result;

	float hp = static_cast<Monster*>(m_pOwner)->GetStatus()->m_Guage;

	std::random_device rd;
	std::mt19937 mersenne(rd());
	std::uniform_int_distribution<> die(2000, 15000);

	if (hp > 70.f) // first phase 
	{ 
		// ������ �������
		result = ENUM_BOSSSKILL::BOSSSKILL_DOWNSTROKE; 
	}
	else if (hp > 40.f)  // second phase
	{
		std::uniform_int_distribution<> monstertype(0, 1);
		/*
		BOSSSKILL_BREATH,
		BOSSSKILL_DOWNSTROKE   
		*/
		result = ENUM_BOSSSKILL(monstertype(mersenne)); // �극�� Ȥ�� �������
	}
	else  // last phase
	{
		std::uniform_int_distribution<> monstertype(1, 4);
		/*
		BOSSSKILL_BREATH,
		BOSSSKILL_DOWNSTROKE 
		BOSSSKILL_TAILATTACK
		BOSSSKILL_RUSH
		*/
		result = ENUM_BOSSSKILL(monstertype(mersenne)); // ������⸦ ������ ����
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
	SoundManager::GetInstance()->Play(ENUM_SOUND::BOSS_MOVE_SOUND);

	XMFLOAT3 toPlayer = Vector3::Normalize(
		Vector3::Subtract(PlayerManager::GetMainPlayer()->GetTransform().GetPosition(), m_pOwner->GetTransform().GetPosition())
	);

	movement->m_xmf3Velocity = Vector3::ScalarProduct(toPlayer, movement->m_fDistance * 2.5f, false);

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
	ENUM_BOSSSKILL skill = GetRandomSkill(); 
	// �÷��̾� ��ų�� ���� �̵� �Ÿ��� �����ɴϴ�. 
	// ���� �÷��̾�� �ش� �Ÿ� ���ϸ� ���¸� ��ȯ���� �ʽ��ϴ�.
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