#pragma once
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
	MonsterAction*  m_CurrMonsterAction  { nullptr };
	MonsterAction*  m_IdleAction         { nullptr };
	MonsterAction*  m_MoveAction         { nullptr };
	MonsterAction*  m_ChaseAction        { nullptr };
	MonsterAction*  m_SearchAction       { nullptr };
	MonsterAction*  m_DeadAction         { nullptr };

	float m_TotalTime{ 0.f };
	const float m_IdleTime{ 1.f }; //Idle 상태로 있는 시간
	const float m_MoveTime{ 6.f }; //Move 상태로 있는 시간

public:   
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

	// 이동에 관련된 호출
	virtual void Update(float) override;

	// Update 수행 이전 반드시 호출
	 void UpdateState(float fElpasedTime);
	 
	void MoveVelocity(const XMFLOAT3& shift);

	void ReduceVelocity(float);
	 
};