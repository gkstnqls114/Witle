#pragma once
#include "Action.h"

class PlayerMovement;
class PlayerActionMgr;

// ���� �׼��� �θ� Ŭ����.
class PlayerAction
	: public Action
{
public:
	// �ʱ�ȭ �Լ�.
	virtual void Init() = 0;

	// ���ӵ��� ����.
	// Scene���� Update ���� ���� �ݵ�� ȣ���ؾ��մϴ�.
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override = 0;
	  
	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) = 0;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	PlayerAction(GameObject* pOwner) : Action(pOwner) {};
	virtual ~PlayerAction() {};

};
