#pragma once
#include "PlayerAction.h"
#include "MonsterAction.h"
 
//// 플레이어 액션 에러 전용 클래스 ////////////////////////////////
class PlayerNoneAction
	: public PlayerAction
{
public:
	virtual void Init() override {};

	// 가속도를 설정.
	// Scene에서 Update 수행 이전 반드시 호출해야합니다.
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override {};

	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) override {};

public: 
	PlayerNoneAction(Player* pOwner) :
		PlayerAction(pOwner, ENUM_PLAYER_ACTIONID::PLAYER_ACTIONID_ERROR, ENUM_ANIMATIONID::ANIMATIONID_ERROR) {};
	virtual ~PlayerNoneAction() {};
};
//// 플레이어 액션 에러 전용 클래스 ////////////////////////////////



//// 기본 몬스터 액션 에러 전용 클래스 ////////////////////////////////
class GeneralMonsterNoneAction
	: public GeneralMonsterAction
{
public:
	virtual void Init() override {};

	// 가속도를 설정.
	// Scene에서 Update 수행 이전 반드시 호출해야합니다.
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override {};

	// 해당 액션상태일 때, 조건을 만족하면 actionMgr을 호출하여 상태를 전환합니다.
	virtual void UpdateState(float fElpasedTime, GeneralMonsterActionMgr* actionMgr) override {};

public:
	GeneralMonsterNoneAction(GameObject* pOwner) :
		GeneralMonsterAction(pOwner, ENUM_GENERALMONSTER_ACTIONID::GENERALMONSTER_ACTIONID_ERROR, ENUM_ANIMATIONID::ANIMATIONID_ERROR) {};
	virtual ~GeneralMonsterNoneAction() {};
};
//// 기본 몬스터 액션 에러 전용 클래스 ////////////////////////////////



//// 보스 몬스터 액션 에러 전용 클래스 ////////////////////////////////
class BossMonsterNoneAction
	: public BossMonsterAction
{
public:
	virtual void Init() override {};

	// 가속도를 설정.
	// Scene에서 Update 수행 이전 반드시 호출해야합니다.
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override {};

	// 해당 액션상태일 때, 조건을 만족하면 actionMgr을 호출하여 상태를 전환합니다.
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override {};

public:
	BossMonsterNoneAction(GameObject* pOwner) :
		BossMonsterAction(pOwner, ENUM_BOSSMONSTER_ACTIONID::BOSSMONSTER_ACTIONID_ERROR, ENUM_ANIMATIONID::ANIMATIONID_ERROR) {};
	virtual ~BossMonsterNoneAction() {};
};
//// 기본 몬스터 액션 에러 전용 클래스 ////////////////////////////////
