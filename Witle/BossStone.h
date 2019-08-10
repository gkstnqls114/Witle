#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// 보스몬스터용 대기 액션
// 움직이지 않습니다.
// 만약 UpdateState 시 , 플레이어와 거리가 500 안으로 들어오면 플레이어에게 따라갑니다.
class BossStone
	: public BossMonsterAction
{
	const float m_IdleTime{ 3.f }; //Idle 상태로 있는 시간

public:
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	BossStone(GameObject* pOwner, float idleTime) : BossMonsterAction(pOwner), m_IdleTime(idleTime) { };
	virtual ~BossStone() {};

};