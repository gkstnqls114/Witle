#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// 기본 몬스터용 죽음 액션
// 움직이지 않습니다.
class DeadAction
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
	DeadAction(GameObject* pOwner) : GeneralMonsterAction(pOwner, GENERALMONSTER_ACTIONID_DEAD, GENERALMONSTER_ANIMATIONID_DEAD) {};
	virtual ~DeadAction() {}; 
};