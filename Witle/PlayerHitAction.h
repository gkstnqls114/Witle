#pragma once
#include "PlayerAction.h"

class MonsterMovement;
class PlayerActionMgr;

// 플레이어 데미지 맞는 액션 
// 현재 액션의 애니메이션이 끝날 때까지 움직일 수 없습니다.
// 액션이 끝난후(= 애니메이션이 끝난 후) Idle 액션으로 전환합니다.
class PlayerHitAction
	: public PlayerAction
{
	const float m_HitFullTime{ ANIMATION_HIT.EndTime - ANIMATION_HIT.StartTime }; //Hit 상태로 있는 시간
	float m_HitElapsedTime{ 0.f };

public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override { m_HitElapsedTime = 0.f; };
	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) override;

public:
	PlayerHitAction(Player* pOwner) : PlayerAction(pOwner, PLAYER_ACTIONID_HIT, PLAYER_ANIMATIONID_HIT), m_HitFullTime(ANIMATION_HIT.EndTime - ANIMATION_HIT.StartTime) { };
	virtual ~PlayerHitAction() {};

};