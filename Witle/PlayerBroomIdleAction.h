#pragma once
#include "PlayerAction.h"

class MonsterMovement;
class PlayerActionMgr;

// 플레이어 빗자루 대기 액션 
// 움직이지않습니다. 가속도 변환을 하지 않습니다.
// 현재 해당 클래스의  UpdateState 함수가 아닌
// PlayerActionMgr 클래스의 함수인 UpdateState 에서 액션을 변환 하고있습니다.
class PlayerBroomIdleAction
	: public PlayerAction
{
	const float m_IdleTime{ 3.f }; //Idle 상태로 있는 시간

public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) override;

public:
	PlayerBroomIdleAction(Player* pOwner, float idleTime) : PlayerAction(pOwner, PLAYER_ACTIONID_BROOMIDLE, PLAYER_ANIMATIONID_BROOMIDLE), m_IdleTime(idleTime) { };
	virtual ~PlayerBroomIdleAction() {};

};