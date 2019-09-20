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
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override;
	virtual void UpdateState(float fElpasedTime, GeneralMonsterActionMgr* actionMgr) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};
	 
public:
	MoveAction(GameObject* pOwner, float idleTime) : GeneralMonsterAction(pOwner, GENERALMONSTER_ACTIONID_MOVE, GENERALMONSTER_ANIMATIONID_MOVE), m_MoveTime(idleTime) {};
	virtual ~MoveAction() {};
	 
	void SetDirection(const XMFLOAT3& direction) { m_Direction = direction; }
	XMFLOAT3 GetDirection() const { return m_Direction; }
};