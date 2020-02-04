#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// 기본 몬스터용 이동 액션
// 필드를 랜덤한 방향으로 돌아다닙니다. 일정 시간이 지나면 Idle로 전환합니다.
// 플레이어와 일정 거리안에 일정시간동안 있다면 Chase로 전환합니다. 
class MoveAction
	: public GeneralMonsterAction
{
	XMFLOAT3 m_Direction{ 0.f, 0.f, 0.f }; // 움직이는 방향
	const float m_MoveTime{ 3.f }; //Move 상태로 있는 시간

public:
	// Update 수행 이전 반드시 호출
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override;
	virtual void UpdateState(float fElpasedTime, GeneralMonsterActionMgr* actionMgr) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};
	 
public:
	MoveAction(GameObject* pOwner, float idleTime) : GeneralMonsterAction(pOwner, GENERALMONSTER_ACTIONID_MOVE, GENERALMONSTER_ANIMATIONID_MOVE), m_MoveTime(idleTime) {};
	virtual ~MoveAction() {};
	 
	void SetDirection(const XMFLOAT3& direction) { m_Direction = direction; }
	XMFLOAT3 GetDirection() const { return m_Direction; }
};