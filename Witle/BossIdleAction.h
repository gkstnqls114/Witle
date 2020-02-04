#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// 보스몬스터용 대기 액션
// 움직이지 않습니다.
// 만약 플레이어와 거리가 일정 안으로 들어와 시간이 지나면 Chase 액션으로 전환합니다.
class BossIdleAction
	: public BossMonsterAction
{
	const float m_IdleTime{ 3.f }; //Idle 상태로 있는 시간

public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	BossIdleAction(GameObject* pOwner, float idleTime) : BossMonsterAction(pOwner, BOSSMONSTER_ACTIONID_IDLE, BOSSMONSTER_ANIMATIONID_IDLE), m_IdleTime(idleTime) { };
	virtual ~BossIdleAction() {};

};