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
	
// 몬스터 액션 매니저를 통해 현재 상태를 통해 (MonsterActionMgr 클래스 참고)
// Movement가 갖고 있는 가속도를 설정합니다.
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

	// 설정된 가속도를 통해 이동합니다.
	virtual void Update(float) override;

	// 액션 매니저를 통해 현재 상태에 따라 가속도를 설정하고 액션을 관리합니다.
	// Update 수행 이전 반드시 호출해야합니다.
	 void UpdateState(float fElpasedTime);
	 
	 // shift만큼 현재 가속도를 추가합니다. 
	void MoveVelocity(const XMFLOAT3& shift);

	// 가속도를 현재 가속도의 0.2 에 비례하여 감소시킵니다.
	// 가속도 크기가 0이라면 감소하지 않습니다.
	void ReduceVelocity(float);

	// 현재 사용하고 있는 액션 매니저 포인터를 가져옵니다.
	MonsterActionMgr* GetMonsterActionMgr() const { return m_MonsterActionMgr; }
};