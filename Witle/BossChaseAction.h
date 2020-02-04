#pragma once
#include "MonsterAction.h"
#include "GameScene.h"

class MonsterMovement;

// 보스몬스터용 쫓아다니기 액션 
// 플레이어를 쫓아다닙니다.
// 쫓아다니는 중 거리가 일정 안으로 들어오면 스킬 액션으로 전환합니다.
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