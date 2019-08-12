#include "stdafx.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Monster.h"
#include "RecognitionRange.h"
#include "MonsterMovement.h"
#include "BossMonsterActionMgr.h"
#include "BossDead.h"

void BossDead::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	// �������� �ʵ��� ����
	movement->m_xmf3Velocity = XMFLOAT3(0.f, 0.f, 0.f);
}

void BossDead::UpdateState(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{ 
}
