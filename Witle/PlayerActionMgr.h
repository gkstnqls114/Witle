#pragma once 
#include "MonsterActionMgr.h"

class PlayerAction;
class PlayerMovement;
class Player;

class PlayerActionMgr
	: public ActionMgr
{
protected:
	// ���� ����ϰ� �ִ� �׼�
	PlayerAction*  m_CurrMonsterAction{ nullptr };

	// ������ ����ߴ� �׼�
	PlayerAction*  m_BeforeMonsterAction{ nullptr };
	 
public:
	virtual void UpdateState(float fElpasedTime) override;
	 
public:
	PlayerActionMgr(GameObject* pOwner) : ActionMgr(pOwner) { };
	virtual ~PlayerActionMgr() {}; 
};