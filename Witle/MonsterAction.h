#pragma once
#include "ComponentBase.h"

class MonsterMovement;
class MonsterActionMgr;
class BossMonsterActionMgr;
class GeneralMonsterActionMgr;
 
class MonsterAction
	: public ComponentBase
{
public:
	virtual void Init() = 0;
	// Update 수행 이전 반드시 호출
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) = 0; 

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	MonsterAction(GameObject* pOwner) : ComponentBase(pOwner) {};
	virtual ~MonsterAction() {}; 
};


class BossMonsterAction
	: public MonsterAction
{ 
public: 
	virtual void Init() override = 0;
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override = 0;
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) = 0;

public:
	BossMonsterAction(GameObject* pOwner) : MonsterAction(pOwner) {};
	virtual ~BossMonsterAction() {};
};


class GeneralMonsterAction
	: public MonsterAction
{
public: 
	virtual void Init() override = 0;
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override  = 0;
	virtual void UpdateState(float fElpasedTime, GeneralMonsterActionMgr* actionMgr) = 0;

public:
	GeneralMonsterAction(GameObject* pOwner) : MonsterAction(pOwner) {};
	virtual ~GeneralMonsterAction() {};
};