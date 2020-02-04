#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// 보스몬스터용 꼬리 공격 스킬 액션 
// 움직이지 않고 애니메이션을 수행합니다.
class BossSkillTailAttack
	: public BossMonsterAction
{
public:
	// Update 수행 이전 반드시 호출
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	BossSkillTailAttack(GameObject* pOwner) : BossMonsterAction(pOwner, BOSSMONSTER_ACTIONID_TAILATTACK, BOSSMONSTER_ANIMATIONID_TAILATTACK) {};
	virtual ~BossSkillTailAttack() {};
};