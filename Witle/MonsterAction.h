#pragma once

class MonsterMovement;

// MonsterMovement�� ���ڷ� �޾� ���ӵ��� ó�����ִ� Ŭ����.
class MonsterAction
{  
public:
	MonsterAction() {};
	~MonsterAction() {};
	 
	// Update ���� ���� �ݵ�� ȣ��
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) = 0;
	virtual void Init() = 0;

	XMFLOAT3 AlreadyUpdate(float) {};
	 
	void MoveVelocity(const XMFLOAT3& shift) {};

	void ReduceVelocity(float) {};
	 
};