#pragma once
#include "ComponentBase.h"

class MonsterAction;

// MonsterMovement�� ���ڷ� �޾� ���ӵ��� ó�����ִ� Ŭ����.
class MonsterActionMgr
	: public ComponentBase
{

	MonsterAction*  m_CurrMonsterAction{ nullptr };
	MonsterAction*  m_IdleAction{ nullptr };
	MonsterAction*  m_MoveAction{ nullptr };
	MonsterAction*  m_ChaseAction{ nullptr };
	MonsterAction*  m_SearchAction{ nullptr };
	MonsterAction*  m_DeadAction{ nullptr };

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	MonsterActionMgr(GameObject* pOwner) : ComponentBase(pOwner) {};
	virtual ~MonsterActionMgr() {};

	// Update ���� ���� �ݵ�� ȣ��
	virtual void UpdateVelocity(float fElpasedTime, MonsterAction* movement) = 0;
	virtual void Init() = 0;

	XMFLOAT3 AlreadyUpdate(float) {};

	void MoveVelocity(const XMFLOAT3& shift) {};

	void ReduceVelocity(float) {};

};