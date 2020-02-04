#pragma once
#include "MonsterAction.h"

class Movement;

// �⺻ ���Ϳ� ���� �׼�
// �ִϸ��̼��� ���� ������ �������� �ʽ��ϴ�.
// �ִϸ��̼��� ���� �� Chase �׼����� ��ȯ�մϴ�.
class AttackAction
	: public GeneralMonsterAction
{
public:
	// Update ���� ���� �ݵ�� ȣ��
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, GeneralMonsterActionMgr* actionMgr) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	AttackAction(GameObject* pOwner) : GeneralMonsterAction(pOwner, GENERALMONSTER_ACTIONID_ATTACK, GENERALMONSTER_ANIMATIONID_ATTACK) {};
	virtual ~AttackAction() {};
};