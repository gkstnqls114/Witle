#pragma once 
#include "PlayerIdleAction.h"
#include "PlayerRightWalkAction.h"
#include "PlayerLeftWalkAction.h"
#include "PlayerForwardWalkAction.h"
#include "PlayerBackwardWalkAction.h"

#include "ActionMgr.h"

class PlayerAction;
class PlayerMovement;
class Player;

class PlayerActionMgr
	: public ActionMgr
{ 
	PlayerIdleAction m_IdleAction;
	PlayerRightWalkAction m_RightWalkAction;
	PlayerLeftWalkAction m_LeftWalkAction;
	PlayerForwardWalkAction m_ForwardWalkAction;
	PlayerBackwardWalkAction m_BackwardWalkAction;

public:
	virtual void UpdateState(float fElpasedTime) override;
	 
public:
	PlayerActionMgr(GameObject* pOwner) : ActionMgr(pOwner) 
	 , m_IdleAction(pOwner, 0) 
		, m_RightWalkAction(pOwner, 0)
		, m_LeftWalkAction(pOwner, 0)
		, m_ForwardWalkAction(pOwner, 0)
		, m_BackwardWalkAction(pOwner, 0) 
	{

	};
	virtual ~PlayerActionMgr() {}; 
};