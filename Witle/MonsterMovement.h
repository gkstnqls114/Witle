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
	
// Movement가 갖고 있는 가속도를 설정합니다.
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

	// 설정된 가속도를 통해 이동합니다.
	virtual void Update(float) override;
	 
	 // shift만큼 현재 가속도를 추가합니다. 
	void MoveVelocity(const XMFLOAT3& shift);

	// 가속도를 현재 가속도의 0.2 에 비례하여 감소시킵니다.
	// 가속도 크기가 0이라면 감소하지 않습니다.
	void ReduceVelocity(float);

};