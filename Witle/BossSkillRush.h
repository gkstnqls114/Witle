#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// �������Ϳ� ���� ��ų �׼� 
// �÷��̾��� �������� ���� ������ �մϴ�.
class BossSkillRush
	: public BossMonsterAction
{
public:
	// Update ���� ���� �ݵ�� ȣ��
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override;
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	BossSkillRush(GameObject* pOwner) : BossMonsterAction(pOwner, BOSSMONSTER_ACTIONID_RUSH, BOSSMONSTER_ANIMATIONID_RUSH) {};
	virtual ~BossSkillRush() {};
};