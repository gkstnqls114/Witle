#pragma once
#include "PlayerAction.h"

class MonsterMovement;
class PlayerActionMgr;

// �÷��̾� ��(-z��) �̵� �׼� 
// ���� �ش� Ŭ������  UpdateVelocity �Լ��� �ƴ�
// PlayerActionMgr Ŭ������ �Լ��� UpdateVelocity ���� ���ӵ� ��ȯ�� �ϰ��ֽ��ϴ�.
class PlayerBackwardWalkAction
	: public PlayerAction
{
	const float m_IdleTime{ 3.f }; //Idle ���·� �ִ� �ð�

public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) override;

public:
	PlayerBackwardWalkAction(Player* pOwner, float idleTime) : PlayerAction(pOwner, PLAYER_ACTIONID_BACKWARD, PLAYER_ANIMATIONID_BACKWARD), m_IdleTime(idleTime) { };
	virtual ~PlayerBackwardWalkAction() {};

};