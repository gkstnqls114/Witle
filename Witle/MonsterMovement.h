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
	MonsterActionMgr* m_MonsterActionMgr { nullptr };
	 
private: 
	void UpdateVelocity(float);

public:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override {};

public:
	MonsterMovement(GameObject* pOwner, float idleTime, float moveTime, bool isBoss = false);
	virtual ~MonsterMovement();

	// 이동에 관련된 호출
	virtual void Update(float) override;

	// Update 수행 이전 반드시 호출
	 void UpdateState(float fElpasedTime);
	 
	void MoveVelocity(const XMFLOAT3& shift);

	void ReduceVelocity(float);
	MonsterActionMgr* GetMonsterActionMgr() const { return m_MonsterActionMgr; }
};