#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// 보스몬스터용 버프 액션
// 움직이지 않습니다.
// 현재 이 액션은 사용하지 않습니다.
class BossBuf
	: public BossMonsterAction
{ 
public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	BossBuf(GameObject* pOwner ) : BossMonsterAction(pOwner, BOSSMONSTER_ACTIONID_BUF, BOSSMONSTER_ANIMATIONID_BUF)  { };
	virtual ~BossBuf() {};

};