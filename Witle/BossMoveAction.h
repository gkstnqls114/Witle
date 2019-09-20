#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// 플레이어를 따라갑니다.
// 만약 UpdateState 시 , 플레이어와 거리가 500 밖으로 넘어가면 플레이어에게 따라갑니다.
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