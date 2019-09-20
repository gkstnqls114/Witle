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
	const float m_fBreathDistance = 2000.f;
	const float m_fDownStrokeDistance = 500.f;
	const float m_fTailAttackDistance = 500.f;
	const float m_fRushDistance = 1500.f;

	
private:
	// 체력에 따라 스킬을 랜덤으로 가져옵니다.
	ENUM_BOSSSKILL GetRandomSkill();
	float GetDistance(ENUM_BOSSSKILL skill);
 
public:
	// Update 수행 이전 반드시 호출
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	BossChaseAction(GameObject* pOwner) : BossMonsterAction(pOwner, BOSSMONSTER_ACTIONID_CHASE, BOSSMONSTER_ANIMATIONID_CHASE) {};
	virtual ~BossChaseAction() {};
};