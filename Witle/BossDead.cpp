#include "stdafx.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Monster.h"
#include "RecognitionRange.h"
#include "MonsterMovement.h"
#include "BossMonsterActionMgr.h"
#include "BossDead.h"
#include "SoundManager.h"

void BossDead::UpdateVelocity(float fElpasedTime, Movement * movement)
{
	// �������� �ʵ��� ����
	movement->SetVelocity( XMFLOAT3(0.f, 0.f, 0.f)); 
}

void BossDead::UpdateState(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{ 
}
