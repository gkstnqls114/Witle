#pragma once
#include "Movement.h"

class PlayerActionMgr;

// 플레이어에 이동에 필요한 가속도, 회전량에 대한 설정
class PlayerMovement
	: public Movement
{
public: 

	bool m_isBroomMode{ false };
	 
private: 
	void MoveVelocity(const XMFLOAT3& shift);

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	PlayerMovement(GameObject* pOwner);
	virtual ~PlayerMovement();
	 
	void MoveVelocity(DWORD dwDirection , float );
	void ReduceVelocity(float);

	void BroomMode();
	void RunMode();
};