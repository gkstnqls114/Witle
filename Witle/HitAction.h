#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// �⺻ ���Ϳ� ������ �޴� �׼�
// �ִϸ��̼��� ���� ������ �������� �ʽ��ϴ�.
// �ִϸ��̼��� ���� �� ������ �ߴ� �׼����� ��ȯ�մϴ� (���� ��� ������ Move���ٸ� Move �׼����� ��ȯ�մϴ�.)
class HitAction
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
	HitAction(GameObject* pOwner) : GeneralMonsterAction( pOwner, GENERALMONSTER_ACTIONID_HIT, GENERALMONSTER_ANIMATIONID_HIT) {};
	virtual ~HitAction() {};
};