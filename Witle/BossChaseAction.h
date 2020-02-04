#pragma once
#include "MonsterAction.h"
#include "GameScene.h"

class MonsterMovement;

// �������Ϳ� �Ѿƴٴϱ� �׼� 
// �÷��̾ �Ѿƴٴմϴ�.
// �Ѿƴٴϴ� �� �Ÿ��� ���� ������ ������ ��ų �׼����� ��ȯ�մϴ�.
class BossChaseAction
	: public BossMonsterAction
{
private:
	const float m_fBreathDistance = 2000.f;
	const float m_fDownStrokeDistance = 500.f;
	const float m_fTailAttackDistance = 500.f;
	const float m_fRushDistance = 1500.f;

	
private:
	// ü�¿� ���� ��ų�� �������� �����ɴϴ�.
	ENUM_BOSSSKILL GetRandomSkill();
	float GetDistance(ENUM_BOSSSKILL skill);
 
public:
	// Update ���� ���� �ݵ�� ȣ��
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	BossChaseAction(GameObject* pOwner) : BossMonsterAction(pOwner, BOSSMONSTER_ACTIONID_CHASE, BOSSMONSTER_ANIMATIONID_CHASE) {};
	virtual ~BossChaseAction() {};
};