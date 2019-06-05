#pragma once 
#include "MonsterActionMgr.h"

class MonsterAction;
class MonsterMovement;
class Player;

// MonsterMovement를 인자로 받아 가속도를 처리해주는 클래스.
class BossMonsterActionMgr
	: public MonsterActionMgr
{ 
	float m_TotalTime{ 0.f };
	 
public:
	virtual void UpdateState(float fElpasedTime) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};
	
public:
	BossMonsterActionMgr(GameObject* pOwner) : MonsterActionMgr(pOwner) { };
	virtual ~BossMonsterActionMgr() {};

	float GetTotalTime() const { return m_TotalTime; }
	void SetZeroTotalTime() { m_TotalTime = 0.f; }
	 
};