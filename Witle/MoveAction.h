#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// MonsterMovement�� ���ڷ� �޾� ���ӵ��� ó�����ִ� Ŭ����.
// �ʵ带 ������ �������� ���ƴٴѴ�.
class MoveAction
	: public MonsterAction
{
public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	MoveAction(GameObject* pOwner) : MonsterAction(pOwner) {};
	virtual ~MoveAction() {};

	// Update ���� ���� �ݵ�� ȣ��
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override;
	virtual void Init() override {};

};