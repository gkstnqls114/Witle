#pragma once
#include "PlayerAction.h"

class Movement;
class PlayerActionMgr;

// �÷��̾� ��� �׼� 
// ���� �ش� Ŭ������  UpdateVelocity �Լ��� �ƴ�
// PlayerActionMgr Ŭ������ �Լ��� UpdateVelocity ���� ���ӵ� ��ȯ�� �ϰ��ֽ��ϴ�.
class PlayerLeftWalkAction
	: public PlayerAction
{
	const float m_IdleTime{ 3.f }; //Idle ���·� �ִ� �ð�

public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) override;

public:
	PlayerLeftWalkAction(Player* pOwner, float idleTime) : PlayerAction(pOwner, PLAYER_ACTIONID_LEFT, PLAYER_ANIMATIONID_LEFT), m_IdleTime(idleTime) { };
	virtual ~PlayerLeftWalkAction() {};

};