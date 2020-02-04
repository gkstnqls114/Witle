#pragma once
#include "PlayerAction.h"

class MonsterMovement;
class PlayerActionMgr;

// �÷��̾� ���� �׼�  
// �̵����� �ʽ��ϴ�.
// UpdateState �Լ������� �ִϸ��̼��� ���� ���� Ȯ�� �� �� Lose Scene���� �̵��մϴ�.
class PlayerDeadAction
	: public PlayerAction
{
	const float m_IdleTime{ 3.f }; //Idle ���·� �ִ� �ð�

public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) override;

public:
	PlayerDeadAction(Player* pOwner, float idleTime) : PlayerAction(pOwner, PLAYER_ACTIONID_DEAD, PLAYER_ANIMATIONID_DEAD), m_IdleTime(idleTime) { };
	virtual ~PlayerDeadAction() {};

};