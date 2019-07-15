#pragma once
#include "MonsterActionMgr.h"
#include "Movement.h"

class Player;

class MonsterAction;

class IdleAction;
class MoveAction;
class ChaseAction;
class SearchAction;
class DeadAction;
	
// ���� �׼� �Ŵ����� ���� ���� ���¸� ���� (MonsterActionMgr Ŭ���� ����)
// Movement�� ���� �ִ� ���ӵ��� �����մϴ�.
class MonsterMovement
	: public Movement
{
	MonsterActionMgr* m_MonsterActionMgr { nullptr };
	 
private: 
	void UpdateVelocity(float);

public:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override {};

public:
	MonsterMovement(GameObject* pOwner, float idleTime, float moveTime, bool isBoss = false);
	virtual ~MonsterMovement();

	// ������ ���ӵ��� ���� �̵��մϴ�.
	virtual void Update(float) override;

	// �׼� �Ŵ����� ���� ���� ���¿� ���� ���ӵ��� �����ϰ� �׼��� �����մϴ�.
	// Update ���� ���� �ݵ�� ȣ���ؾ��մϴ�.
	 void UpdateState(float fElpasedTime);
	 
	 // shift��ŭ ���� ���ӵ��� �߰��մϴ�. 
	void MoveVelocity(const XMFLOAT3& shift);

	// ���ӵ��� ���� ���ӵ��� 0.2 �� ����Ͽ� ���ҽ�ŵ�ϴ�.
	// ���ӵ� ũ�Ⱑ 0�̶�� �������� �ʽ��ϴ�.
	void ReduceVelocity(float);

	// ���� ����ϰ� �ִ� �׼� �Ŵ��� �����͸� �����ɴϴ�.
	MonsterActionMgr* GetMonsterActionMgr() const { return m_MonsterActionMgr; }
};