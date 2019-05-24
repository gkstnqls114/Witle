#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// MonsterMovement�� ���ڷ� �޾� ���ӵ��� ó�����ִ� Ŭ����.
// �������� �ʰ� ó���� �Ѵ�.
class IdleAction
	: public MonsterAction
{
public:
	IdleAction() {};
	~IdleAction() {};

	// Update ���� ���� �ݵ�� ȣ��
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override;
	virtual void Init() override {};
};