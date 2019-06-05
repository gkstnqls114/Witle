#pragma once
#include "ComponentBase.h"

class MonsterMovement;
class MonsterActionMgr;

// MonsterMovement를 인자로 받아 가속도를 처리해주는 클래스.
class MonsterAction
	: public ComponentBase
{
public:
	// Update 수행 이전 반드시 호출
	virtual void Init() = 0;
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) = 0;
	virtual void UpdateState(float fElpasedTime, MonsterActionMgr* actionMgr) = 0;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	MonsterAction(GameObject* pOwner) : ComponentBase(pOwner) {};
	virtual ~MonsterAction() {};
	 
	XMFLOAT3 AlreadyUpdate(float) {};
	 
	void MoveVelocity(const XMFLOAT3& shift) {};

	void ReduceVelocity(float) {}; 
};


class BossMonsterAction
	: public MonsterAction
{ 
public:
	// Update 수행 이전 반드시 호출
	virtual void Init() = 0;
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) = 0;
	virtual void UpdateState(float fElpasedTime, MonsterActionMgr* actionMgr) = 0;

public:
	BossMonsterAction(GameObject* pOwner) : MonsterAction(pOwner) {};
	virtual ~BossMonsterAction() {};
};