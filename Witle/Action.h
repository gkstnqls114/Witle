#pragma once
#include "ComponentBase.h"
 
class MonsterMovement;

// �׼��� �θ� Ŭ����.
class Action
	: public ComponentBase
{
public:
	// �ʱ�ȭ �Լ�.
	virtual void Init() = 0;

	// ���ӵ��� ����.
	// Scene���� Update ���� ���� �ݵ�� ȣ���ؾ��մϴ�.
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) = 0;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	Action(GameObject* pOwner) : ComponentBase(pOwner) {};
	virtual ~Action() {};
};
