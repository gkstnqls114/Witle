#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// �⺻ ���Ϳ� �÷��̾� ã�� �׼�
// ���� ������� �ʽ��ϴ�.
class SearchAction
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
	SearchAction(GameObject* pOwner) : GeneralMonsterAction(pOwner, GENERALMONSTER_ACTIONID_SEARCH, GENERALMONSTER_ANIMATIONID_MOVE) {};
	virtual ~SearchAction() {};

};