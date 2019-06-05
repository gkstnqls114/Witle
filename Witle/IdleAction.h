#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// MonsterMovement�� ���ڷ� �޾� ���ӵ��� ó�����ִ� Ŭ����.
// �������� �ʰ� ó���� �Ѵ�.
class IdleAction
	: public GeneralMonsterAction
{
	const float m_IdleTime{ 3.f }; //Idle ���·� �ִ� �ð�
	 
public:
	// Update ���� ���� �ݵ�� ȣ��
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, GeneralMonsterActionMgr* actionMgr) override;

public:
	IdleAction(GameObject* pOwner, float idleTime) : GeneralMonsterAction(pOwner), m_IdleTime(idleTime) { };
	virtual ~IdleAction() {};

};