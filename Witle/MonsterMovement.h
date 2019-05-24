#pragma once
#include "ComponentBase.h"

class Player;

class MonsterAction;

class IdleAction;
class MoveAction;
class ChaseAction;
class SearchAction;
class DeadAction;
	
// �÷��̾ �̵��� �ʿ��� ���ӵ�, ȸ������ ���� ����
class MonsterMovement
	: public ComponentBase
{
	MonsterAction*  m_CurrMonsterAction  { nullptr };
	MonsterAction*  m_IdleAction         { nullptr };
	MonsterAction*  m_MoveAction         { nullptr };
	MonsterAction*  m_ChaseAction        { nullptr };
	MonsterAction*  m_SearchAction       { nullptr };
	MonsterAction*  m_DeadAction         { nullptr };

	float m_TotalTime{ 0.f };
	const float m_IdleTime{ 1.f }; //Idle ���·� �ִ� �ð�
	const float m_MoveTime{ 6.f }; //Move ���·� �ִ� �ð�

public:  
	XMFLOAT3 m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float m_fMaxVelocityY{ 400.0f };
	float m_fMaxVelocityXZ{ 2000.0f };
	float m_fFriction = 1700.0f;
	float m_fDistance = 2000.f; // 20m / ��

	// ȸ����
	float m_fPitch = 0.0f;
	float m_fRoll = 0.0f;
	float m_fYaw = 0.0f;

	const float m_fNearDistance{ 0.f };
 
private:
	bool IsNearPlayer(const Player* Target, float distance);
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
	virtual void UpdateState(float fElpasedTime);

	XMFLOAT3 AlreadyUpdate(float);
	 
	void MoveVelocity(const XMFLOAT3& shift);

	void ReduceVelocity(float);
	 
};