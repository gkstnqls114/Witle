#include "stdafx.h"
#include "GameObject.h"
#include "Player.h"
#include "PlayerManager.h"
#include "Monster.h"
#include "Object.h"
#include "MonsterMovement.h"
#include "BossMonsterActionMgr.h"
#include "BossSkillRush.h"

void BossSkillRush::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	movement->m_xmf3Velocity = XMFLOAT3(0, 0, 0);
}

void BossSkillRush::Init()
{
	// �ٶ󺸴� ���� �ѹ� �����Ѵ�.

	XMFLOAT3 toPlayer = Vector3::Normalize(
		Vector3::Subtract(
			PlayerManager::GetMainPlayer()->GetTransform().GetPosition(),
			m_pOwner->GetTransform().GetPosition()
		)
	);

	MonsterMovement* movement = static_cast<Monster*>(m_pOwner)->GetMovement();

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


void BossSkillRush::UpdateState(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{
	// Attack �ִϸ��̼��� ������ �ʾ��� ��� Chnage State ���� �ʴ´�.
	if (!static_cast<Monster*>(m_pOwner)->GetpLoadObject()->IsTrackAnimationSetFinish(0, BOSS_RUSH.ID)) return;

	actionMgr->ChangeBossStateToChase();
}