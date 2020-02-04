#include "stdafx.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Monster.h"
#include "RecognitionRange.h"
#include "PlayerMovement.h"
#include "PlayerActionMgr.h"

#include "SoundManager.h"

#include "PlayerBroomIdleAction.h"

void PlayerBroomIdleAction::UpdateVelocity(float fElpasedTime, Movement * movement)
{ 
	// �������� �ʵ��� ����
	movement->SetVelocity(XMFLOAT3(0.f, 0.f, 0.f));
}

void PlayerBroomIdleAction::UpdateState(float fElpasedTime, PlayerActionMgr * actionMgr)
{

}
