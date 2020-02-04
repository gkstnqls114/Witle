#pragma once
#include "PlayerAction.h"

class MonsterMovement;
class PlayerActionMgr;

// 플레이어 빗자루 이동(+z축/-z축) 액션  
// 현재 해당 클래스의  UpdateState, UpdateVelocity 함수가 아닌
// PlayerActionMgr 클래스의 함수인 UpdateState, UpdateVelocity 에서 가속도와 상태 변환을 하고있습니다.
class PlayerBroomForwardAction
	: public PlayerAction
{
	const float m_IdleTime{ 3.f }; //Idle 상태로 있는 시간

public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) override;

public:
	PlayerBroomForwardAction(Player* pOwner, float idleTime) : PlayerAction(pOwner, PLAYER_ACTIONID_BROOMFORWARD, PLAYER_ANIMATIONID_BROOMFORWARD), m_IdleTime(idleTime) { };
	virtual ~PlayerBroomForwardAction() {};

};