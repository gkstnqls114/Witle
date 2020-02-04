#pragma once
#include "PlayerAction.h"

class MonsterMovement;
class PlayerActionMgr;

// 플레이어 빗자루 준비 액션 
// 현재 액션의 애니메이션이 끝날 때까지 움직일 수 없습니다.
// 액션이 끝난후(= 애니메이션이 끝난 후) BroomIdle 액션으로 전환합니다.
class PlayerBroomPrepareAction
	: public PlayerAction
{
	const float m_IdleTime{ 3.f }; //Idle 상태로 있는 시간

public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) override;

public:
	PlayerBroomPrepareAction(Player* pOwner, float idleTime) : PlayerAction(pOwner, PLAYER_ACTIONID_BROOMPREPARE, PLAYER_ANIMATIONID_BROOMPREPARE), m_IdleTime(idleTime) { };
	virtual ~PlayerBroomPrepareAction() {};

};