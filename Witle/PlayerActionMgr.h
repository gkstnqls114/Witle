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

#include "NoneAction.h"

#include "ActionMgr.h"

class PlayerAction;
class PlayerMovement;
class Player;

class PlayerActionMgr
	: public ActionMgr
{
	PlayerNoneAction	       m_PlayerErrorAction;

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

	bool m_isDead{ false };

public:
	virtual void UpdateState(float fElpasedTime) override;
	 
	// ���Ŀ� ����� �׼� ID�� ���� ����ϴ� �׼� ID �ٸ��� �˾Ƴ���. �ٸ��ٸ� true
	virtual bool isDifferAfterAndCurrent() const override;

	// AfterAction�� �� Ŭ������ �´� NoneAction���� �����Ѵ�.
	virtual void SetUpAfterAction() override;

	// AfterAction�� �� Ŭ������ �´� NoneAction�̶�� true�� ��ȯ�Ѵ�.
	virtual bool isAfterNoneAction() override;

	// m_pOwner�� SetTrackAnimationSet �Լ��� ȣ���մϴ�.
	virtual void SetTrackAnimationSet() const override;

	virtual void Init() override;

private:
	// ���ڷ糪 ��ų�� ������� �ʰ� �޸��ų� ���ִ� ����
	bool isRunMode() const;
	bool isBroomMode() const;
	
public: 
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	PlayerActionMgr(Player* pOwner);
	virtual ~PlayerActionMgr() {}; 

	virtual void UpdateVelocity(float fElpasedTime, Movement* const movement)override;

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
	 
	PlayerAction* GetCurrPlayerAction() const 
	{
		return static_cast<PlayerAction*>(m_CurrAction); 
	};
	
	PlayerAction* GetBeforePlayerAction() const 
	{
		return static_cast<PlayerAction*>(m_BeforeAction);
	};

	PlayerAction* GetAfterPlayerAction() const
	{
		return static_cast<PlayerAction*>(m_AfterAction);
	};
};