#pragma once
#include "ActionMgr.h"
#include "Movement.h"

class Player;

class Action;

class IdleAction;
class MoveAction;
class ChaseAction;
class SearchAction;
class DeadAction;
	
// Movement�� ���� �ִ� ���ӵ��� �����մϴ�.
class MonsterMovement
	: public Movement
{
private: 

public:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override {};

public:
	MonsterMovement(GameObject* pOwner);
	virtual ~MonsterMovement();

	// ������ ���ӵ��� ���� �̵��մϴ�.
	virtual void Update(float) override;
	 
	 // shift��ŭ ���� ���ӵ��� �߰��մϴ�. 
	void MoveVelocity(const XMFLOAT3& shift);

	// ���ӵ��� ���� ���ӵ��� 0.2 �� ����Ͽ� ���ҽ�ŵ�ϴ�.
	// ���ӵ� ũ�Ⱑ 0�̶�� �������� �ʽ��ϴ�.
	void ReduceVelocity(float);

};