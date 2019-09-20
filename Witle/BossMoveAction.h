#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// �÷��̾ ���󰩴ϴ�.
// ���� UpdateState �� , �÷��̾�� �Ÿ��� 500 ������ �Ѿ�� �÷��̾�� ���󰩴ϴ�.
class BossMoveAction
	: public BossMonsterAction
{
	XMFLOAT3 m_Direction{ 0.f, 0.f, 0.f }; // �����̴� ����
	const float m_MoveTime{ 3.f }; //Move ���·� �ִ� �ð�

public:
	// Update ���� ���� �ݵ�� ȣ��
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	BossMoveAction(GameObject* pOwner, float idleTime) : BossMonsterAction(pOwner, BOSSMONSTER_ACTIONID_MOVE, BOSSMONSTER_ANIMATIONID_MOVE), m_MoveTime(idleTime) { };
	virtual ~BossMoveAction() {};

	void SetDirection(const XMFLOAT3& direction) { m_Direction = direction; }
	XMFLOAT3 GetDirection() const { return m_Direction; }
};