#pragma once
#include "MonsterAction.h"

class MonsterMovement;
 
class ChaseAction
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
	ChaseAction(GameObject* pOwner) : GeneralMonsterAction(pOwner, GENERALMONSTER_ACTIONID_CHASE, SPACECAT_MOVE.ID) {};
	virtual ~ChaseAction() {};
};