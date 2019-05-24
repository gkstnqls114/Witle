#pragma once
#include "ComponentBase.h"

class MonsterAction;

class IdleAction;
class MoveAction;
class ChaseAction;
class SearchAction;
class DeadAction;
	
// 플레이어에 이동에 필요한 가속도, 회전량에 대한 설정
class MonsterMovement
	: public ComponentBase
{
	MonsterAction*  m_CurrMonsterAction  { nullptr };
	MonsterAction*  m_IdleAction         { nullptr };
	MonsterAction*  m_MoveAction         { nullptr };
	MonsterAction*  m_ChaseAction        { nullptr };
	MonsterAction*  m_SearchAction       { nullptr };
	MonsterAction*  m_DeadAction         { nullptr };

public:  
	XMFLOAT3 m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float m_fMaxVelocityY{ 400.0f };
	float m_fMaxVelocityXZ{ 2000.0f };
	float m_fFriction = 1700.0f;
	float m_fDistance = 2000.f; // 20m / 초

	// 회전량
	float m_fPitch = 0.0f;
	float m_fRoll = 0.0f;
	float m_fYaw = 0.0f;

	const float m_fNearDistance{ 0.f };

private:
	bool IsNearPlayer(const GameObject* Target, float distance);
	void UpdateVelocity(float);

public:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override {};

public:
	MonsterMovement(GameObject* pOwner);
	virtual ~MonsterMovement();

	// 이동에 관련된 호출
	virtual void Update(float) override;

	// Update 수행 이전 반드시 호출
	virtual void UpdateState(float fElpasedTime, const XMFLOAT3& randomDirection, const GameObject* pTarget, float distance);

	XMFLOAT3 AlreadyUpdate(float);

	void MoveVelocity(DWORD dwDirection, float);
	void MoveVelocity(const XMFLOAT3& shift);

	void ReduceVelocity(float);

	void BroomMode();
	void RunMode();
};