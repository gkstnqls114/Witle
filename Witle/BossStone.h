#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// �������Ϳ� ��� �׼�
// �������� �ʽ��ϴ�.
// ���� UpdateState �� , �÷��̾�� �Ÿ��� 500 ������ ������ �÷��̾�� ���󰩴ϴ�.
class BossStone
	: public BossMonsterAction
{
	const float m_IdleTime{ 3.f }; //Idle ���·� �ִ� �ð�

public:
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	BossStone(GameObject* pOwner, float idleTime) : BossMonsterAction(pOwner), m_IdleTime(idleTime) { };
	virtual ~BossStone() {};

};