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

#include "ErrorAction.h"

#include "ActionMgr.h"

class PlayerAction;
class PlayerMovement;
class Player;

class PlayerActionMgr
	: public ActionMgr
{ 
	PlayerErrorAction	       m_PlayerErrorAction;

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

	// 이전과 현재의 Action ID 상태가 달라졌는지 알아낸다. 만약 다르다면 true
	virtual bool isDifferAction() const override;

	// 이후에 사용할 액션 ID와 현재 사용하는 액션 ID 다른지 알아낸다. 다르다면 true
	virtual bool isDifferAfterAndCurrent() const override;

public: 
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	PlayerActionMgr(GameObject* pOwner) : ActionMgr(pOwner) 
		, m_PlayerErrorAction(pOwner)
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
		Init();
	};
	virtual ~PlayerActionMgr() {}; 

	virtual void Init() override;

	bool Is_IdleAction() const { return (m_AfterAction == &m_IdleAction); }
	bool Is_RightWalkAction() const { return (m_AfterAction == &m_RightWalkAction); }
	bool Is_LeftWalkAction() const { return (m_AfterAction == &m_LeftWalkAction); }
	bool Is_ForwardWalkAction() const { return (m_AfterAction == &m_ForwardWalkAction); }
	bool Is_BackwardWalkAction() const { return (m_AfterAction == &m_BackwardWalkAction); }
	bool Is_StandardAttackAction() const { return (m_AfterAction == &m_StandardAttackAction); }
	bool Is_BroomPrepareAction() const { return (m_AfterAction == &m_BroomPrepareAction); }
	bool Is_BroomIdleAction() const { return (m_AfterAction == &m_BroomIdleAction); }
	bool Is_BroomForwardAction() const { return (m_AfterAction == &m_BroomForwardAction); }
	bool Is_DeadAction() const { return (m_AfterAction == &m_DeadAction); }
	bool Is_HitAction() const { return (m_AfterAction == &m_HitAction); }

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
	 
	PlayerAction* GetCurrPlayerAction() const 
	{
		return static_cast<PlayerAction*>(m_AfterAction); 
	};
	
	PlayerAction* GetBeforePlayerAction() const 
	{
		return static_cast<PlayerAction*>(m_BeforeAction);
	};
};