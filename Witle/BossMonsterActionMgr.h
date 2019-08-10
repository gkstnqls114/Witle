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
	BossBuf m_BossBuf;
	BossDead m_BossDead;
	BossStone m_BossStone;
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
		m_BossBuf(pOwner),
		m_BossDead(pOwner),
		m_BossStone(pOwner)
	{
		m_CurrMonsterAction = &m_BossIdleAction;
	};
	virtual ~BossMonsterActionMgr() {};

	float GetTotalTime() const { return m_TotalTime; }
	void SetZeroTotalTime() { m_TotalTime = 0.f; }
	 
	bool Is_BossIdleAction() const { return (m_CurrMonsterAction == &m_BossIdleAction); } // �⺻
	bool Is_BossMoveAction() const { return (m_CurrMonsterAction == &m_BossMoveAction); } // ������
	bool Is_BossChaseAction() const { return (m_CurrMonsterAction == &m_BossChaseAction); } // �߰�
	bool Is_BossSkillBreath() const { return (m_CurrMonsterAction == &m_BossSkillBreath); }// �극��
	bool Is_BossSkillDownStroke() const { return (m_CurrMonsterAction == &m_BossSkilldownstroke); }// �������
	bool Is_BossSkillTailAttack() const { return (m_CurrMonsterAction == &m_BossSkIllTailAttack); }// ����ȸ��
	bool Is_BossSkillRush() const { return (m_CurrMonsterAction == &m_BossSkIlldash); }// ����
	
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
