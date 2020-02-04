#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// 기본 몬스터용 데미지 받는 액션
// 애니메이션이 끝날 때까지 움직이지 않습니다.
// 애니메이션이 끝난 후 이전에 했던 액션으로 전환합니다 (예를 들어 이전이 Move였다면 Move 액션으로 전환합니다.)
class HitAction
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
	HitAction(GameObject* pOwner) : GeneralMonsterAction( pOwner, GENERALMONSTER_ACTIONID_HIT, GENERALMONSTER_ANIMATIONID_HIT) {};
	virtual ~HitAction() {};
};