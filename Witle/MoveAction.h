#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// MonsterMovement�� ���ڷ� �޾� ���ӵ��� ó�����ִ� Ŭ����.
// �ʵ带 ������ �������� ���ƴٴѴ�.
class MoveAction
	: public GeneralMonsterAction
{
	XMFLOAT3 m_Direction{ 0.f, 0.f, 0.f }; // �����̴� ����
	const float m_MoveTime{ 3.f }; //Move ���·� �ִ� �ð�

public:
	// Update ���� ���� �ݵ�� ȣ��
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override;
	virtual void Init() override;
	virtual void UpdateState(float fElpasedTime, GeneralMonsterActionMgr* actionMgr) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};
	 
public:
	MoveAction(GameObject* pOwner) : GeneralMonsterAction(pOwner) {};
	virtual ~MoveAction() {};
	 
	void SetDirection(const XMFLOAT3& direction) { m_Direction = direction; }
	XMFLOAT3 GetDirection() const { return m_Direction; }
};