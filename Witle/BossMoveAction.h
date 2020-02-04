#pragma once
#include "MonsterAction.h"

class MonsterMovement;
 
// 보스몬스터용 움직이기 액션 
// 랜덤한 방향으로 움직입니다.
// 만약 플레이어와의 거리가 일정 안으로 들어와 일정 시간이 지나면 Chase 액션으로 전환합니다.
class BossMoveAction
	: public BossMonsterAction
{
	XMFLOAT3 m_Direction{ 0.f, 0.f, 0.f }; // 움직이는 방향
	const float m_MoveTime{ 3.f }; //Move 상태로 있는 시간

public:
	// Update 수행 이전 반드시 호출
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	BossMoveAction(GameObject* pOwner, float idleTime) : BossMonsterAction(pOwner, BOSSMONSTER_ACTIONID_MOVE, BOSSMONSTER_ANIMATIONID_MOVE), m_MoveTime(idleTime) { };
	virtual ~BossMoveAction() {};

	void SetDirection(const XMFLOAT3& direction) { m_Direction = direction; }
	XMFLOAT3 GetDirection() const { return m_Direction; }
};