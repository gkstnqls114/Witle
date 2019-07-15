#pragma once
#include "ComponentBase.h"

class MonsterMovement;
class MonsterActionMgr;
class BossMonsterActionMgr;
class GeneralMonsterActionMgr;
 
// 몬스터 액션의 부모 클래스.
class MonsterAction
	: public ComponentBase
{
public:
	// 초기화 함수.
	virtual void Init() = 0; 

	// 가속도를 설정.
	// Scene에서 Update 수행 이전 반드시 호출해야합니다.
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) = 0; 

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	MonsterAction(GameObject* pOwner) : ComponentBase(pOwner) {};
	virtual ~MonsterAction() {}; 
};


// 보스 몬스터용 액션의 부모 클래스.
// 보스 몬스터용 액션은 해당 액션을 상속받습니다.
class BossMonsterAction
	: public MonsterAction
{ 
public: 
	// 초기화 함수.
	virtual void Init() override = 0;

	// 가속도를 설정.
	// Scene에서 Update 수행 이전 반드시 호출해야합니다.
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override = 0;

	// 해당 액션상태일 때, 조건을 만족하면 actionMgr을 호출하여 상태를 전환합니다.
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) = 0;

public:
	BossMonsterAction(GameObject* pOwner) : MonsterAction(pOwner) {};
	virtual ~BossMonsterAction() {};
};


// 일반 몬스터용 액션의 부모 클래스.
// 일반 몬스터용 액션은 해당 액션을 상속받습니다.
class GeneralMonsterAction
	: public MonsterAction
{
public:
	// 초기화 함수.
	virtual void Init() override = 0;

	// 가속도를 설정.
	// Scene에서 Update 수행 이전 반드시 호출해야합니다.
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override  = 0;

	// 해당 액션상태일 때, 조건을 만족하면 actionMgr을 호출하여 상태를 전환합니다.
	virtual void UpdateState(float fElpasedTime, GeneralMonsterActionMgr* actionMgr) = 0;

public:
	GeneralMonsterAction(GameObject* pOwner) : MonsterAction(pOwner) {};
	virtual ~GeneralMonsterAction() {};
};