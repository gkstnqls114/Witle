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

void BossChaseAction::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	SoundManager::GetInstance()->Play(ENUM_SOUND::BOSS_MOVE_SOUND);

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
	int val = rand() % 5;

	Player* player = PlayerManager::GetMainPlayer();

	if (PlayerManager::IsNearPlayer(m_pOwner->GetTransform().GetPosition(), 150))
	{
		// 체력이 100 ~
		if (static_cast<Monster*>(m_pOwner)->GetStatus()->m_Guage >= 100.f)
		{
			if (val == 0)
			{
				(actionMgr)->ChangeBossStateToAction0();

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
				(actionMgr)->ChangeBossStateToAction1();

				if (static_cast<Monster*>(m_pOwner)->GetisAttacking())
				{
					if (Collision::isCollide(player->GetBOBox()->GetBOBox(), static_cast<Monster*>(m_pOwner)->GetBOBox()->GetBOBox()))
					{
						player->SubstractHP(10);
					}
				}
			}
		}
		// 체력이 70 ~
		else if (static_cast<Monster*>(m_pOwner)->GetStatus()->m_Guage > 70.f)
		{
			if (val == 0)
			{
				(actionMgr)->ChangeBossStateToAction0();

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
				(actionMgr)->ChangeBossStateToAction1();

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
				(actionMgr)->ChangeBossStateToAction2();

				if (static_cast<Monster*>(m_pOwner)->GetisAttacking())
				{
					if (Collision::isCollide(player->GetBOBox()->GetBOBox(), static_cast<Monster*>(m_pOwner)->GetBOBox()->GetBOBox()))
					{
						player->SubstractHP(15);
					}
				}
			}
		}
		// 체력이 30 ~
		else if (static_cast<Monster*>(m_pOwner)->GetStatus()->m_Guage > 30.f)
		{
			if (val == 0)
			{
				(actionMgr)->ChangeBossStateToAction0();

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
				(actionMgr)->ChangeBossStateToAction1();

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
				(actionMgr)->ChangeBossStateToAction2();

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
				(actionMgr)->ChangeBossStateToAction3();

				if (static_cast<Monster*>(m_pOwner)->GetisAttacking())
				{
					if (Collision::isCollide(player->GetBOBox()->GetBOBox(), static_cast<Monster*>(m_pOwner)->GetBOBox()->GetBOBox()))
					{
						player->SubstractHP(20);
					}
				}
			}
		}
		else if (static_cast<Monster*>(m_pOwner)->GetStatus()->m_Guage >= 0.f)
		{
			(actionMgr)->ChangeBossStateToIdle();
		}
	}
}