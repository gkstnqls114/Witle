#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// �������Ϳ� �극�� ��ų �׼� 
// �������� �ʰ� �ִϸ��̼��� �����մϴ�.
// �ִϸ��̼��� ������ ������ ���� ��ġ���� �Ұ�(fireball)�� Ȱ��ȭ�մϴ�.
class BossSkillBreath
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
	BossSkillBreath(GameObject* pOwner) : BossMonsterAction(pOwner, BOSSMONSTER_ACTIONID_BREATH, BOSSMONSTER_ANIMATIONID_BREATH) {};
	virtual ~BossSkillBreath() {};
};