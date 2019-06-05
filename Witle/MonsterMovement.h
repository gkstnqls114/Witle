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
	
// 
class MonsterMovement
	: public Movement
{
	MonsterActionMgr* m_MonsterActionMgr { nullptr };


public:   
	const float m_fNearDistance{ 0.f };
 
private: 
	void UpdateVelocity(float);

public:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override {};

public:
	MonsterMovement(GameObject* pOwner);
	virtual ~MonsterMovement();

	// �̵��� ���õ� ȣ��
	virtual void Update(float) override;

	// Update ���� ���� �ݵ�� ȣ��
	 void UpdateState(float fElpasedTime);
	 
	void MoveVelocity(const XMFLOAT3& shift);

	void ReduceVelocity(float);
	MonsterActionMgr* GetMonsterActionMgr() const { return m_MonsterActionMgr; }
};