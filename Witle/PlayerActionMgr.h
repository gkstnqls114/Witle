#pragma once 
#include "PlayerIdleAction.h"
#include "PlayerRightWalkAction.h"
#include "PlayerLeftWalkAction.h"
#include "PlayerForwardWalkAction.h"
#include "PlayerBackwardWalkAction.h"

#include "PlayerStandardAttackAction.h"
#include "PlayerBroomPrepareAction.h"
#include "PlayerBroomIdleAction.h"
#include "PlayerBroomForwardAction.h"
#include "PlayerDeadAction.h"
#include "PlayerHitAction.h"

#include "ActionMgr.h"

class PlayerAction;
class PlayerMovement;
class Player;

class PlayerActionMgr
	: public ActionMgr
{ 
	PlayerIdleAction           m_IdleAction;
	PlayerRightWalkAction      m_RightWalkAction;
	PlayerLeftWalkAction       m_LeftWalkAction;
	PlayerForwardWalkAction    m_ForwardWalkAction;
	PlayerBackwardWalkAction   m_BackwardWalkAction;

	PlayerStandardAttackAction m_StandardAttackAction;
	PlayerBroomPrepareAction   m_BroomPrepareAction;
	PlayerBroomIdleAction      m_BroomIdleAction;
	PlayerBroomForwardAction   m_BroomForwardAction;
	PlayerDeadAction           m_DeadAction;
	PlayerHitAction            m_HitAction;

public:
	virtual void UpdateState(float fElpasedTime) override;
	 
public: 
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	PlayerActionMgr(GameObject* pOwner) : ActionMgr(pOwner) 
	 , m_IdleAction(pOwner, 0) 
		, m_RightWalkAction(pOwner, 0)
		, m_LeftWalkAction(pOwner, 0)
		, m_ForwardWalkAction(pOwner, 0)
		, m_BackwardWalkAction(pOwner, 0) 
		, m_StandardAttackAction(pOwner, 0)
		, m_BroomPrepareAction(pOwner, 0)
		, m_BroomIdleAction(pOwner, 0)
		, m_BroomForwardAction(pOwner, 0)
		, m_DeadAction(pOwner, 0)
		, m_HitAction(pOwner, 0)
	{

	};
	virtual ~PlayerActionMgr() {}; 

	void Init();

	bool Is_IdleAction() const { return (m_CurrAction == &m_IdleAction); }
	bool Is_RightWalkAction() const { return (m_CurrAction == &m_RightWalkAction); }
	bool Is_LeftWalkAction() const { return (m_CurrAction == &m_LeftWalkAction); }
	bool Is_ForwardWalkAction() const { return (m_CurrAction == &m_ForwardWalkAction); }
	bool Is_BackwardWalkAction() const { return (m_CurrAction == &m_BackwardWalkAction); }
	bool Is_StandardAttackAction() const { return (m_CurrAction == &m_StandardAttackAction); }
	bool Is_BroomPrepareAction() const { return (m_CurrAction == &m_BroomPrepareAction); }
	bool Is_BroomIdleAction() const { return (m_CurrAction == &m_BroomIdleAction); }
	bool Is_BroomForwardAction() const { return (m_CurrAction == &m_BroomForwardAction); }
	bool Is_DeadAction() const { return (m_CurrAction == &m_DeadAction); }
	bool Is_HitAction() const { return (m_CurrAction == &m_HitAction); }

	void ChangeActionToIdle();
	void ChangeActionToRightWalk();
	void ChangeActionToLeftWalk();
	void ChangeActionToForwardWalk();
	void ChangeActionToBackwardWalk();
	void ChangeActionToStandardAttack();
	void ChangeActionToBroomPrepare();
	void ChangeActionToBroomIdle();
	void ChangeActionToBroomForward();
	void ChangeActionToDead();
	void ChangeActionToHit(); 
	 
};