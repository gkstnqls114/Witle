#pragma once
#include "PlayerAction.h"

class MonsterMovement;
class PlayerActionMgr;

// 보스몬스터용 대기 액션
// 움직이지 않습니다.
// 만약 UpdateState 시 , 플레이어와 거리가 500 안으로 들어오면 플레이어에게 따라갑니다.
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