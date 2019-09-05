#pragma once
#include "MonsterActionMgr.h"
#include "Movement.h"

class Player;

class Action;

class IdleAction;
class MoveAction;
class ChaseAction;
class SearchAction;
class DeadAction;
	
// ���� �׼� �Ŵ����� ���� ���� ���¸� ���� (ActionMgr Ŭ���� ����)
// Movement�� ���� �ִ� ���ӵ��� �����մϴ�.
class MonsterMovement
	: public Movement
{
	ActionMgr* m_MonsterActionMgr { nullptr };
	 
private: 

public:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override {};

public:
	MonsterMovement(GameObject* pOwner, float idleTime, float moveTime, bool isBoss = false);
	virtual ~MonsterMovement();

	// ������ ���ӵ��� ���� �̵��մϴ�.
	virtual void Update(float) override;

	// Update ���� ���� �ݵ�� ȣ���ؾ��մϴ�.
	 void UpdateState(float fElpasedTime);
	 
	 // �׼� �Ŵ����� ���� ���� ���¿� ���� ���ӵ��� �����ϰ� �׼��� �����մϴ�.
	 void UpdateVelocity(float);

	 // shift��ŭ ���� ���ӵ��� �߰��մϴ�. 
	void MoveVelocity(const XMFLOAT3& shift);

	// ���ӵ��� ���� ���ӵ��� 0.2 �� ����Ͽ� ���ҽ�ŵ�ϴ�.
	// ���ӵ� ũ�Ⱑ 0�̶�� �������� �ʽ��ϴ�.
	void ReduceVelocity(float);

	// ���� ����ϰ� �ִ� �׼� �Ŵ��� �����͸� �����ɴϴ�.
	ActionMgr* GetMonsterActionMgr() const { return m_MonsterActionMgr; }
};