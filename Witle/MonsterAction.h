#pragma once
#include "Action.h"

class MonsterMovement;
class ActionMgr;
class BossMonsterActionMgr;
class GeneralMonsterActionMgr;
 
// 보스 몬스터용 액션의 부모 클래스.
// 보스 몬스터용 액션은 해당 액션을 상속받습니다.
class BossMonsterAction
	: public Action
{
public:
	const ENUM_BOSSMONSTER_ACTIONID m_ActionID{ BOSSMONSTER_ACTIONID_ERROR };

public: 
	// 초기화 함수.
	virtual void Init() override = 0;

	// 가속도를 설정.
	// Scene에서 Update 수행 이전 반드시 호출해야합니다.
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override = 0;

	// 해당 액션상태일 때, 조건을 만족하면 actionMgr을 호출하여 상태를 전환합니다.
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) = 0;

public:
	BossMonsterAction(GameObject* pOwner, ENUM_BOSSMONSTER_ACTIONID actionID, ENUM_ANIMATIONID animeID)
		: Action(pOwner, animeID) , m_ActionID(actionID) {};
	virtual ~BossMonsterAction() {};
};


// 일반 몬스터용 액션의 부모 클래스.
// 일반 몬스터용 액션은 해당 액션을 상속받습니다.
class GeneralMonsterAction
	: public Action
{
public:
	const ENUM_GENERALMONSTER_ACTIONID m_ActionID{ GENERALMONSTER_ACTIONID_ERROR };

public:
	// 초기화 함수.
	virtual void Init() override = 0;

	// 가속도를 설정.
	// Scene에서 Update 수행 이전 반드시 호출해야합니다.
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override  = 0;

	// 해당 액션상태일 때, 조건을 만족하면 actionMgr을 호출하여 상태를 전환합니다.
	virtual void UpdateState(float fElpasedTime, GeneralMonsterActionMgr* actionMgr) = 0;

public:
	GeneralMonsterAction(GameObject* pOwner, ENUM_GENERALMONSTER_ACTIONID actionID, ENUM_ANIMATIONID animeID)
		: Action(pOwner, animeID), m_ActionID(actionID){};
	virtual ~GeneralMonsterAction() {};
};