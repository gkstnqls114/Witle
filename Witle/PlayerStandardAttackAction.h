#pragma once
#include "PlayerAction.h"

class MonsterMovement;
class PlayerActionMgr;

// 플레이어 공격 액션 
// 현재 액션의 애니메이션이 끝날 때까지 움직일 수 없습니다.
// 액션이 끝난후(= 애니메이션이 끝난 후) Idle 액션으로 전환합니다.
class PlayerStandardAttackAction
	: public PlayerAction
{
	const float m_IdleTime{ 3.f }; //Idle 상태로 있는 시간

public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) override;

public:
	PlayerStandardAttackAction(Player* pOwner, float idleTime) : PlayerAction(pOwner, PLAYER_ACTIONID_ATTACK, PLAYER_ANIMATIONID_ATTACK), m_IdleTime(idleTime) { };
	virtual ~PlayerStandardAttackAction() {};

};