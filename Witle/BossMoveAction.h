#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// 보스몬스터용 샘플 액션 2입니다.
// 플레이어를 따라갑니다.
// 만약 UpdateState 시 , 플레이어와 거리가 500 밖으로 넘어가면 플레이어에게 따라갑니다.
class BossMoveAction
	: public BossMonsterAction
{

public:
	// Update 수행 이전 반드시 호출
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	BossMoveAction(GameObject* pOwner) : BossMonsterAction(pOwner) { };
	virtual ~BossMoveAction() {};

};