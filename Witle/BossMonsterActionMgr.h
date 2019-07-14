#pragma once 
#include "BossSampleAction_1.h"
#include "BossSampleAction_2.h"
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
	BossSampleAction_1 m_BossSampleAction_1;
	BossSampleAction_2 m_BossSampleAction_2;
	// ���� ����ϴ� �׼�(����)��

public:
	virtual void UpdateState(float fElpasedTime) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};
	
public:
	BossMonsterActionMgr(GameObject* pOwner) : 
		MonsterActionMgr(pOwner),
		m_BossSampleAction_1(pOwner),
		m_BossSampleAction_2(pOwner)
	{
	};
	virtual ~BossMonsterActionMgr() {};

	void ChangeStateToSample_1();
	void ChangeStateToSample_2();
	void BossIdleAction(); // �⺻
	void BossMoveAction(); // ������
	void BossChaseAction(); // �߰�
	void BossSearchAction(); // �ʿ��ұ�?
	void BossDeadAction(); // ����
	void BossSkillAction0(); // �ӽ�
	void BossSkillAction1(); // �ӽ�
	void BossSkillAction2(); // �ӽ�
	void BossSkillAction3(); // �ӽ�
	void BossSkillAction4(); // �ӽ�
	void BossSkillAction5(); // �ӽ�
	void BossSkillAction6(); // �ӽ�
	void BossSkillAction7(); // �ӽ�
	void BossSkillAction8(); // �ӽ�
	void BossSkillAction9(); // �ӽ�
};
