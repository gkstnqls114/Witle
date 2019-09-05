#pragma once 
#include "MonsterActionMgr.h"

class PlayerAction;
class PlayerMovement;
class Player;

class PlayerActionMgr
	: public ActionMgr
{
protected:
	// 현재 사용하고 있는 액션
	PlayerAction*  m_CurrMonsterAction{ nullptr };

	// 이전에 사용했던 액션
	PlayerAction*  m_BeforeMonsterAction{ nullptr };
	 
public:
	virtual void UpdateState(float fElpasedTime) override;
	 
public:
	PlayerActionMgr(GameObject* pOwner) : ActionMgr(pOwner) { };
	virtual ~PlayerActionMgr() {}; 
};