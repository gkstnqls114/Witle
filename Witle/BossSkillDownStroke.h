#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// �������Ϳ� ����ġ�� ��ų �׼� 
// �������� �ʰ� �ִϸ��̼��� �����մϴ�.
class BossSkillDownStroke
	: public BossMonsterAction
{
public:
	// Update ���� ���� �ݵ�� ȣ��
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	BossSkillDownStroke(GameObject* pOwner) : BossMonsterAction(pOwner, BOSSMONSTER_ACTIONID_DOWNSTROKE, BOSSMONSTER_ANIMATIONID_DOWNSTROKE) {};
	virtual ~BossSkillDownStroke() {};
};