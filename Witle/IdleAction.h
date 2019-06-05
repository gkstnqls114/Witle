#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// MonsterMovement를 인자로 받아 가속도를 처리해주는 클래스.
// 움직이지 않게 처리를 한다.
class IdleAction
	: public MonsterAction
{
public:
	// Update 수행 이전 반드시 호출
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, MonsterActionMgr* actionMgr) override;

public:
	IdleAction(GameObject* pOwner) : MonsterAction(pOwner) {};
	virtual ~IdleAction() {};

};