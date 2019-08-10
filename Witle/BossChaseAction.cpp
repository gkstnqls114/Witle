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
	
	if (hp > 70.f) // first phase 
	{
		/*
		BOSSSKILL_BREATH,
		BOSSSKILL_DOWNSTROKE �� �ϳ�
		*/
		result = ENUM_BOSSSKILL(rand() % 2); // 0 Ȥ�� 1
	}
	else if (hp > 30.f)  // second phase
	{
		/*
		BOSSSKILL_BREATH,
		BOSSSKILL_DOWNSTROKE  
		BOSSSKILL_TAILATTACK
		*/
		result = ENUM_BOSSSKILL(rand() % 3); // 0 Ȥ�� 1 
	}
	else  // last phase
	{ 
		/*
		BOSSSKILL_BREATH,
		BOSSSKILL_DOWNSTROKE 
		BOSSSKILL_TAILATTACK
		BOSSSKILL_RUSH
		*/
		result = ENUM_BOSSSKILL(rand() % 4); // 0 Ȥ�� 1
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

	return 0.0f;
}

bool BossChaseAction::UpdateFirstPhase(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{
	float hp = static_cast<Monster*>(m_pOwner)->GetStatus()->m_Guage;

	// hp�� 70�̻��� ��쿡�� 1 ������
	if (!(hp > 70.f)) return false;

	int val = rand() % 2;
	Player* player = PlayerManager::GetMainPlayer();

	(actionMgr)->ChangeBossStateToRush(); // �Ÿ� 700������ �׷����غ��ϵ�

	// �׽�Ʈ ���� �ּ�ó��
/*
	if (val == 0)
	{
		(actionMgr)->ChangeBossStateToBreth();

		if (static_cast<Monster*>(m_pOwner)->GetisAttacking())
		{
			if (Collision::isCollide(player->GetBOBox(), static_cast<Monster*>(m_pOwner)->GetBOBox()))
			{
				player->SubstractHP(5);
			}
		}
	}
	else if (val == 1)
	{
		(actionMgr)->ChangeBossStateToDownStroke();

		if (static_cast<Monster*>(m_pOwner)->GetisAttacking())
		{
			if (Collision::isCollide(player->GetBOBox(), static_cast<Monster*>(m_pOwner)->GetBOBox()))
			{
				player->SubstractHP(10);
			}
		}
	}*/

	return true;
}

bool BossChaseAction::UpdateSecondPhase(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{
	float hp = static_cast<Monster*>(m_pOwner)->GetStatus()->m_Guage;

	// hp�� 30�̻��� ��쿡�� 1 ������
	if (!(hp > 30.f)) return false;

	int val = rand() % 2;
	Player* player = PlayerManager::GetMainPlayer();

	if (val == 0)
	{
		(actionMgr)->ChangeBossStateToBreth();

		if (static_cast<Monster*>(m_pOwner)->GetisAttacking())
		{
			if (Collision::isCollide(player->GetBOBox(), static_cast<Monster*>(m_pOwner)->GetBOBox()))
			{
				player->SubstractHP(5);
			}
		}
	}
	else if (val == 1)
	{
		(actionMgr)->ChangeBossStateToDownStroke();

		if (static_cast<Monster*>(m_pOwner)->GetisAttacking())
		{
			if (Collision::isCollide(player->GetBOBox(), static_cast<Monster*>(m_pOwner)->GetBOBox()))
			{
				player->SubstractHP(10);
			}
		}
	}
	else if (val == 2)
	{
		(actionMgr)->ChangeBossStateToTailAttack();

		if (static_cast<Monster*>(m_pOwner)->GetisAttacking())
		{
			if (Collision::isCollide(player->GetBOBox(), static_cast<Monster*>(m_pOwner)->GetBOBox()))
			{
				player->SubstractHP(15);
			}
		}
	}

	return true;
}

bool BossChaseAction::UpdateLastPhase(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{
	int val = rand() % 4;
	Player* player = PlayerManager::GetMainPlayer();

	if (val == 0)
	{
		(actionMgr)->ChangeBossStateToBreth();

		if (static_cast<Monster*>(m_pOwner)->GetisAttacking())
		{
			if (Collision::isCollide(player->GetBOBox(), static_cast<Monster*>(m_pOwner)->GetBOBox()))
			{
				player->SubstractHP(5);
			}
		}
	}
	else if (val == 1)
	{
		(actionMgr)->ChangeBossStateToDownStroke();

		if (static_cast<Monster*>(m_pOwner)->GetisAttacking())
		{
			if (Collision::isCollide(player->GetBOBox(), static_cast<Monster*>(m_pOwner)->GetBOBox()))
			{
				player->SubstractHP(5);
			}
		}
	}
	else if (val == 2)
	{
		(actionMgr)->ChangeBossStateToTailAttack();

		if (static_cast<Monster*>(m_pOwner)->GetisAttacking())
		{
			if (Collision::isCollide(player->GetBOBox(), static_cast<Monster*>(m_pOwner)->GetBOBox()))
			{
				player->SubstractHP(15);
			}
		}
	}
	else if (val == 3)
	{
		(actionMgr)->ChangeBossStateToRush();

		if (static_cast<Monster*>(m_pOwner)->GetisAttacking())
		{
			if (Collision::isCollide(player->GetBOBox(), static_cast<Monster*>(m_pOwner)->GetBOBox()))
			{
				player->SubstractHP(20);
			}
		}
	} 

	return true;
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
	// ���� ü���� 0��� ���¸� 
	if (static_cast<Monster*>(m_pOwner)->GetStatus()->m_Guage <= 0.f)
	{
		(actionMgr)->ChangeBossStateToIdle(); 
	}
	  
	ENUM_BOSSSKILL skill = GetRandomSkill(); 
	// �÷��̾� ��ų�� ���� �̵� �Ÿ��� �����ɴϴ�. 
	// ���� �÷��̾�� �ش� �Ÿ� ���ϸ� ���¸� ��ȯ���� �ʽ��ϴ�.
	if (!PlayerManager::IsNearPlayer(m_pOwner->GetTransform().GetPosition(), GetDistance(skill))) return;

	bool isFirst = UpdateFirstPhase(fElpasedTime, actionMgr);
	if (isFirst) return;

	bool isSecond = UpdateSecondPhase(fElpasedTime, actionMgr);
	if (isSecond) return;

	UpdateLastPhase(fElpasedTime, actionMgr); 
}