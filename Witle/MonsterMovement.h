#pragma once
#include "MonsterActionMgr.h"
#include "Movement.h"

class Player;

class MonsterAction;

class IdleAction;
class MoveAction;
class ChaseAction;
class SearchAction;
class DeadAction;
	
// 
class MonsterMovement
	: public Movement
{
	GeneralMonsterActionMgr* m_MonsterActionMgr { nullptr };

public:   
	const float m_fNearDistance{ 0.f };
 
private: 
	void UpdateVelocity(float);

public:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override {};

public:
	MonsterMovement(GameObject* pOwner, float idleTime, float moveTime);
	virtual ~MonsterMovement();

	// 이동에 관련된 호출
	virtual void Update(float) override;

	// Update 수행 이전 반드시 호출
	 void UpdateState(float fElpasedTime);
	 
	void MoveVelocity(const XMFLOAT3& shift);

	void ReduceVelocity(float);
	GeneralMonsterActionMgr* GetMonsterActionMgr() const { return m_MonsterActionMgr; }
};