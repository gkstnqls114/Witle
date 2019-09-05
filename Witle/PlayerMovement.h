#pragma once
#include "Movement.h"

class PlayerActionMgr;

// �÷��̾ �̵��� �ʿ��� ���ӵ�, ȸ������ ���� ����
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