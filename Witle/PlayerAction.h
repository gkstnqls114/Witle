#pragma once
#include "ComponentBase.h"

class PlayerMovement;
class PlayerActionMgr;

// ���� �׼��� �θ� Ŭ����.
class PlayerAction
	: public ComponentBase
{
public:
	// �ʱ�ȭ �Լ�.
	virtual void Init() = 0;

	// ���ӵ��� ����.
	// Scene���� Update ���� ���� �ݵ�� ȣ���ؾ��մϴ�.
	virtual void UpdateVelocity(float fElpasedTime, PlayerMovement* movement) = 0;
	  
	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) = 0;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	PlayerAction(GameObject* pOwner) : ComponentBase(pOwner) {};
	virtual ~PlayerAction() {};

};
