#pragma once  
#include "BossIdleAction.h"
#include "BossMoveAction.h"
#include "BossChaseAction.h"
#include"BossSkillBreath.h"
#include"BossSkillDownStroke.h"
#include"BossSkillTailAttack.h"
#include"BossSkillRush.h"
#include"BossSkillAction4.h"
#include "MonsterActionMgr.h"

class MonsterAction;
class MonsterMovement;
class Player;

// ���� ���Ϳ� �׼� �Ŵ����Դϴ�.
// MonsterActionMgr�� ��ӹ޾� ���� �׼ǻ��¸� �����մϴ�. 
class BossMonsterActionMgr
	: public MonsterActionMgr
{
	// ���� ����ϴ� �׼�(����)�� 
	BossIdleAction m_BossIdleAction;
	BossMoveAction m_BossMoveAction;
	BossChaseAction m_BossChaseAction;
	BossSkillBreath m_BossSkillBreath;
	BossSkillDownStroke m_BossSkilldownstroke;
	BossSkillTailAttack m_BossSkIllTailAttack;
	BossSkillRush m_BossSkIlldash;
	BossSkillAction4 m_BossSkillAction4;
	// ���� ����ϴ� �׼�(����)��

	float m_TotalTime{ 0.f };

public:
	virtual void UpdateState(float fElpasedTime) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	BossMonsterActionMgr(GameObject* pOwner, float IdleTime, float MoveTime) :
		MonsterActionMgr(pOwner), 
		m_BossIdleAction(pOwner, IdleTime),
		m_BossMoveAction(pOwner, MoveTime),
		m_BossChaseAction(pOwner),
		m_BossSkillBreath(pOwner),
		m_BossSkilldownstroke(pOwner),
		m_BossSkIllTailAttack(pOwner),
		m_BossSkIlldash(pOwner),
		m_BossSkillAction4(pOwner)
	{
		m_CurrMonsterAction = &m_BossIdleAction;
	};
	virtual ~BossMonsterActionMgr() {};

	float GetTotalTime() const { return m_TotalTime; }
	void SetZeroTotalTime() { m_TotalTime = 0.f; }
	 
	bool Is_BossIdleAction() const { return (m_CurrMonsterAction == &m_BossIdleAction); } // �⺻
	bool Is_BossMoveAction() const { return (m_CurrMonsterAction == &m_BossMoveAction); } // ������
	bool Is_BossChaseAction() const { return (m_CurrMonsterAction == &m_BossChaseAction); } // �߰�
	// bool Is_BossSearchAction() const { return (m_CurrMonsterAction == &m_BossSearchAction); } // �ʿ��ұ�?
	// bool Is_BossDeadAction() const { return (m_CurrMonsterAction == &m_BossDeadAction); } // ����
	bool Is_BossSkillActionBreath() const { return (m_CurrMonsterAction == &m_BossSkillBreath); }// �극��
	bool Is_BossSkillAction1() const { return (m_CurrMonsterAction == &m_BossSkilldownstroke); }// �������
	bool Is_BossSkillActionTailAttack() const { return (m_CurrMonsterAction == &m_BossSkIllTailAttack); }// ����ȸ��
	bool Is_BossSkillActiondash() const { return (m_CurrMonsterAction == &m_BossSkIlldash); }// ����
	bool Is_BossSkillAction4() const { return (m_CurrMonsterAction == &m_BossSkillAction4); }// �ӽ�
	// bool Is_BossSkillAction5() const { return (m_CurrMonsterAction == &m_BossSkillAction5); }// �ӽ�
	// bool Is_BossSkillAction6() const { return (m_CurrMonsterAction == &m_BossSkillAction6); }// �ӽ�
	// bool Is_BossSkillAction7() const { return (m_CurrMonsterAction == &m_BossSkillAction7); }// �ӽ�
	// bool Is_BossSkillAction8() const { return (m_CurrMonsterAction == &m_BossSkillAction8); }// �ӽ�
	// bool Is_BossSkillAction9() const { return (m_CurrMonsterAction == &m_BossSkillAction9); }// �ӽ�

	void ChangeBossStateBefore();
	void ChangeBossStateToIdle();
	void ChangeBossStateToMove();
	void ChangeBossStateToChase();
	void ChangeBossStateToSearch();
	void ChangeBossStateToDead();
	void ChangeBossStateToHit();
	void ChangeBossStateToBreth();
	void ChangeBossStateToDownStroke();
	void ChangeBossStateToAction2();
	void ChangeBossStateToAction3();
	void ChangeBossStateToAction4();
};
