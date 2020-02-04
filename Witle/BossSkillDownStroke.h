#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// 보스몬스터용 내려치기 스킬 액션 
// 움직이지 않고 애니메이션을 수행합니다.
class BossSkillDownStroke
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
	BossSkillDownStroke(GameObject* pOwner) : BossMonsterAction(pOwner, BOSSMONSTER_ACTIONID_DOWNSTROKE, BOSSMONSTER_ANIMATIONID_DOWNSTROKE) {};
	virtual ~BossSkillDownStroke() {};
};