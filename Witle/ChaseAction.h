#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// MonsterMovement를 인자로 받아 가속도를 처리해주는 클래스.
// 필드를 랜덤한 방향으로 돌아다닌다.
class ChaseAction
	: public MonsterAction
{
public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	ChaseAction(GameObject* pOwner) : MonsterAction(pOwner) {};
	virtual ~ChaseAction() {};

	// Update 수행 이전 반드시 호출
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override;
	virtual void Init() override {};

};