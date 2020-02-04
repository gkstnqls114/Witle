#pragma once
#include "MonsterAction.h"

class Movement;

// 기본 몬스터용 공격 액션
// 애니메이션이 끝날 때까지 움직이지 않습니다.
// 애니메이션이 끝난 후 Chase 액션으로 전환합니다.
class AttackAction
	: public GeneralMonsterAction
{
public:
	// Update 수행 이전 반드시 호출
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, GeneralMonsterActionMgr* actionMgr) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	AttackAction(GameObject* pOwner) : GeneralMonsterAction(pOwner, GENERALMONSTER_ACTIONID_ATTACK, GENERALMONSTER_ANIMATIONID_ATTACK) {};
	virtual ~AttackAction() {};
};