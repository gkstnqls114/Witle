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

bool BossChaseAction::UpdateFirstPhase(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{
	float hp = static_cast<Monster*>(m_pOwner)->GetStatus()->m_Guage;

	// hp가 70이상인 경우에는 1 페이즈
	if (!(hp > 70.f)) return false;

	int val = rand() % 2;
	Player* player = PlayerManager::GetMainPlayer();

	if (val == 0)
	{
		(actionMgr)->ChangeBossStateToBreth();

		if (static_cast<Monster*>(m_pOwner)->GetisAttacking())
		{
			if (Collision::isCollide(player->GetBOBox()->GetBOBox(), static_cast<Monster*>(m_pOwner)->GetBOBox()->GetBOBox()))
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
			if (Collision::isCollide(player->GetBOBox()->GetBOBox(), static_cast<Monster*>(m_pOwner)->GetBOBox()->GetBOBox()))
			{
				player->SubstractHP(10);
			}
		}
	}

	return true;
}

bool BossChaseAction::UpdateSecondPhase(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{
	float hp = static_cast<Monster*>(m_pOwner)->GetStatus()->m_Guage;

	// hp가 30이상인 경우에는 1 페이즈
	if (!(hp > 30.f)) return false;

	int val = rand() % 2;
	Player* player = PlayerManager::GetMainPlayer();

	if (val == 0)
	{
		(actionMgr)->ChangeBossStateToBreth();

		if (static_cast<Monster*>(m_pOwner)->GetisAttacking())
		{
			if (Collision::isCollide(player->GetBOBox()->GetBOBox(), static_cast<Monster*>(m_pOwner)->GetBOBox()->GetBOBox()))
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
			if (Collision::isCollide(player->GetBOBox()->GetBOBox(), static_cast<Monster*>(m_pOwner)->GetBOBox()->GetBOBox()))
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
			if (Collision::isCollide(player->GetBOBox()->GetBOBox(), static_cast<Monster*>(m_pOwner)->GetBOBox()->GetBOBox()))
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
			if (Collision::isCollide(player->GetBOBox()->GetBOBox(), static_cast<Monster*>(m_pOwner)->GetBOBox()->GetBOBox()))
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
			if (Collision::isCollide(player->GetBOBox()->GetBOBox(), static_cast<Monster*>(m_pOwner)->GetBOBox()->GetBOBox()))
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
			if (Collision::isCollide(player->GetBOBox()->GetBOBox(), static_cast<Monster*>(m_pOwner)->GetBOBox()->GetBOBox()))
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
			if (Collision::isCollide(player->GetBOBox()->GetBOBox(), static_cast<Monster*>(m_pOwner)->GetBOBox()->GetBOBox()))
			{
				player->SubstractHP(20);
			}
		}
	} 
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
		(actionMgr)->ChangeBossStateToIdle(); 
	}
	 
	// 만약 플레이어와 일정 거리 이하면 상태를 전환하지 않습니다.
	if (!PlayerManager::IsNearPlayer(m_pOwner->GetTransform().GetPosition(), 150)) return;

	bool isFirst = UpdateFirstPhase(fElpasedTime, actionMgr);
	if (isFirst) return;

	bool isSecond = UpdateSecondPhase(fElpasedTime, actionMgr);
	if (isSecond) return;

	UpdateLastPhase(fElpasedTime, actionMgr); 
}