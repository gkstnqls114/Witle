#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// MonsterMovement�� ���ڷ� �޾� ���ӵ��� ó�����ִ� Ŭ����.
// �ʵ带 ������ �������� ���ƴٴѴ�.
class BossSkillTailAttack
	: public BossMonsterAction
{
public:
	// Update ���� ���� �ݵ�� ȣ��
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	BossSkillTailAttack(GameObject* pOwner) : BossMonsterAction(pOwner) {};
	virtual ~BossSkillTailAttack() {};
};