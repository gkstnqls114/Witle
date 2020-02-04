#pragma once
#include "PlayerAction.h"

class MonsterMovement;
class PlayerActionMgr;

// 플레이어 죽음 액션  
// 이동하지 않습니다.
// UpdateState 함수내에서 애니메이션이 끝난 것을 확인 한 후 Lose Scene으로 이동합니다.
class PlayerDeadAction
	: public PlayerAction
{
	const float m_IdleTime{ 3.f }; //Idle 상태로 있는 시간

public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) override;

public:
	PlayerDeadAction(Player* pOwner, float idleTime) : PlayerAction(pOwner, PLAYER_ACTIONID_DEAD, PLAYER_ANIMATIONID_DEAD), m_IdleTime(idleTime) { };
	virtual ~PlayerDeadAction() {};

};