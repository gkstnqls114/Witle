#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// �������Ϳ� ���� ���� ��ų �׼� 
// �������� �ʰ� �ִϸ��̼��� �����մϴ�.
class BossSkillTailAttack
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
	BossSkillTailAttack(GameObject* pOwner) : BossMonsterAction(pOwner, BOSSMONSTER_ACTIONID_TAILATTACK, BOSSMONSTER_ANIMATIONID_TAILATTACK) {};
	virtual ~BossSkillTailAttack() {};
};