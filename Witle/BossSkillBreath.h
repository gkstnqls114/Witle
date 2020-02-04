#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// 보스몬스터용 브레스 스킬 액션 
// 움직이지 않고 애니메이션을 수행합니다.
// 애니메이션이 끝나기 전까지 입의 위치에서 불공(fireball)을 활성화합니다.
class BossSkillBreath
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
	BossSkillBreath(GameObject* pOwner) : BossMonsterAction(pOwner, BOSSMONSTER_ACTIONID_BREATH, BOSSMONSTER_ANIMATIONID_BREATH) {};
	virtual ~BossSkillBreath() {};
};