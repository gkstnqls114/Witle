#pragma once
#include "MonsterAction.h"
#include "GameScene.h"

class MonsterMovement;

// MonsterMovement를 인자로 받아 가속도를 처리해주는 클래스.
// 필드를 랜덤한 방향으로 돌아다닌다.
class BossChaseAction
	: public BossMonsterAction
{
private:
	// 모두 업데이트가 완료된다면 return true를 합니다.
	bool UpdateFirstPhase(float fElpasedTime, BossMonsterActionMgr* actionMgr);
	bool UpdateSecondPhase(float fElpasedTime, BossMonsterActionMgr* actionMgr);
	bool UpdateLastPhase(float fElpasedTime, BossMonsterActionMgr* actionMgr);

public:
	// Update 수행 이전 반드시 호출
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	BossChaseAction(GameObject* pOwner) : BossMonsterAction(pOwner) {};
	virtual ~BossChaseAction() {};
};