#pragma once  
#include "BossIdleAction.h"
#include "BossMoveAction.h"
#include "BossChaseAction.h"
#include"BossSkillBreath.h"
#include"BossSkillDownStroke.h"
#include"BossSkillTailAttack.h"
#include"BossSkillRush.h" 
#include "BossStone.h"
#include "BossDead.h"
#include "BossBuf.h"

#include "NoneAction.h"

#include "ActionMgr.h"

class Action;
class MonsterMovement;
class Player;

// ���� ���Ϳ� �׼� �Ŵ����Դϴ�.
// ActionMgr�� ��ӹ޾� ���� �׼ǻ��¸� �����մϴ�. 
class BossMonsterActionMgr
	: public ActionMgr
{
	// ���� ����ϴ� �׼�(����)�� 
	BossMonsterNoneAction m_BossMonsterErrorAction;

	BossIdleAction m_BossIdleAction;
	BossMoveAction m_BossMoveAction;
	BossChaseAction m_BossChaseAction;
	BossSkillBreath m_BossSkillBreath;
	BossSkillDownStroke m_BossSkilldownstroke;
	BossSkillTailAttack m_BossSkIllTailAttack;
	BossSkillRush m_BossSkIllRush;
	BossBuf m_BossBuf;
	BossDead m_BossDead;
	BossStone m_BossStone;
	// ���� ����ϴ� �׼�(����)��

	float m_TotalTime{ 0.f };

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

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	BossMonsterActionMgr(GameObject* pOwner, float IdleTime, float MoveTime) :
		ActionMgr(pOwner), 
		m_BossMonsterErrorAction(pOwner),
		m_BossIdleAction(pOwner, IdleTime),
		m_BossMoveAction(pOwner, MoveTime),
		m_BossChaseAction(pOwner),
		m_BossSkillBreath(pOwner),
		m_BossSkilldownstroke(pOwner),
		m_BossSkIllTailAttack(pOwner),
		m_BossSkIllRush(pOwner),
		m_BossBuf(pOwner),
		m_BossDead(pOwner),
		m_BossStone(pOwner)
	{ 
		m_BeforeAction = &m_BossMonsterErrorAction;
		m_AfterAction = &m_BossMonsterErrorAction;
		m_CurrAction = &m_BossMonsterErrorAction;
	};
	virtual ~BossMonsterActionMgr() {};

	float GetTotalTime() const { return m_TotalTime; }
	void SetZeroTotalTime() { m_TotalTime = 0.f; }
	 
	bool Is_BossIdleAction() const { return (m_AfterAction == &m_BossIdleAction); } // �⺻
	bool Is_BossMoveAction() const { return (m_AfterAction == &m_BossMoveAction); } // ������
	bool Is_BossChaseAction() const { return (m_AfterAction == &m_BossChaseAction); } // �߰�
	bool Is_BossSkillBreath() const { return (m_AfterAction == &m_BossSkillBreath); }// �극��
	bool Is_BossSkillDownStroke() const { return (m_AfterAction == &m_BossSkilldownstroke); }// �������
	bool Is_BossSkillTailAttack() const { return (m_AfterAction == &m_BossSkIllTailAttack); }// ����ȸ��
	bool Is_BossSkillRush() const { return (m_AfterAction == &m_BossSkIllRush); }// ����
	bool Is_BossDead() const { return (m_AfterAction == &m_BossDead); }// ����
	
	void ChangeBossStateBefore();
	void ChangeBossStateToIdle();
	void ChangeBossStateToMove();
	void ChangeBossStateToChase(); 
	void ChangeBossStateToBreath();
	void ChangeBossStateToDownStroke();
	void ChangeBossStateToTailAttack();
	void ChangeBossStateToRush();
	void ChangeBossStateToStone();
	void ChangeBossStateToDead();
	void ChangeBossStateToBuf();
};
