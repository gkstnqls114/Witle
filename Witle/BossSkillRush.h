#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// 보스몬스터용 러쉬 스킬 액션 
// 플레이어의 방향으로 돌진 공격을 합니다.
class BossSkillRush
	: public BossMonsterAction
{
public:
	// Update 수행 이전 반드시 호출
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override;
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	BossSkillRush(GameObject* pOwner) : BossMonsterAction(pOwner, BOSSMONSTER_ACTIONID_RUSH, BOSSMONSTER_ANIMATIONID_RUSH) {};
	virtual ~BossSkillRush() {};
};