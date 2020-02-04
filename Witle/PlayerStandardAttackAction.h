#pragma once
#include "PlayerAction.h"

class MonsterMovement;
class PlayerActionMgr;

// �÷��̾� ���� �׼� 
// ���� �׼��� �ִϸ��̼��� ���� ������ ������ �� �����ϴ�.
// �׼��� ������(= �ִϸ��̼��� ���� ��) Idle �׼����� ��ȯ�մϴ�.
class PlayerStandardAttackAction
	: public PlayerAction
{
	const float m_IdleTime{ 3.f }; //Idle ���·� �ִ� �ð�

public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) override;

public:
	PlayerStandardAttackAction(Player* pOwner, float idleTime) : PlayerAction(pOwner, PLAYER_ACTIONID_ATTACK, PLAYER_ANIMATIONID_ATTACK), m_IdleTime(idleTime) { };
	virtual ~PlayerStandardAttackAction() {};

};