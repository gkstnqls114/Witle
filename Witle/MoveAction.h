#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// MonsterMovement를 인자로 받아 가속도를 처리해주는 클래스.
// 필드를 랜덤한 방향으로 돌아다닌다.
class MoveAction
	: public MonsterAction
{
	XMFLOAT3 m_Direction{ 0.f, 0.f, 0.f }; // 움직이는 방향

public:
	// Update 수행 이전 반드시 호출
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, MonsterActionMgr* actionMgr) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	MoveAction(GameObject* pOwner) : MonsterAction(pOwner) {};
	virtual ~MoveAction() {};
	 
	void SetDirection(const XMFLOAT3& direction) { m_Direction = direction; }
	XMFLOAT3 GetDirection() const { return m_Direction; }
};