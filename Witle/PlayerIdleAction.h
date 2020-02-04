#pragma once
#include "PlayerAction.h"

class Movement;
class PlayerActionMgr;

// 플레이어 대기 액션 
// 현재 해당 클래스의  UpdateVelocity 함수가 아닌
// PlayerActionMgr 클래스의 함수인 UpdateVelocity 에서 가속도 변환을 하고있습니다.
class PlayerIdleAction
	: public PlayerAction
{
	const float m_IdleTime{ 3.f }; //Idle 상태로 있는 시간

public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) override;

public:
	PlayerIdleAction(Player* pOwner, float idleTime) : PlayerAction(pOwner, PLAYER_ACTIONID_IDLE, PLAYER_ANIMATIONID_IDLE), m_IdleTime(idleTime) { };
	virtual ~PlayerIdleAction() {};

};