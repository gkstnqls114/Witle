#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// 보스몬스터용 샘플 액션 1입니다.
// 움직이지 않습니다.
// 만약 UpdateState 시 , 플레이어와 거리가 500 안으로 들어오면 플레이어에게 따라갑니다.
class BossSampleAction_1
	: public BossMonsterAction
{

public:
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	BossSampleAction_1(GameObject* pOwner) : BossMonsterAction(pOwner) { };
	virtual ~BossSampleAction_1() {};

};