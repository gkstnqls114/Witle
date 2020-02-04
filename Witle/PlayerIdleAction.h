#pragma once
#include "PlayerAction.h"

class Movement;
class PlayerActionMgr;

// �÷��̾� ��� �׼� 
// ���� �ش� Ŭ������  UpdateVelocity �Լ��� �ƴ�
// PlayerActionMgr Ŭ������ �Լ��� UpdateVelocity ���� ���ӵ� ��ȯ�� �ϰ��ֽ��ϴ�.
class PlayerIdleAction
	: public PlayerAction
{
	const float m_IdleTime{ 3.f }; //Idle ���·� �ִ� �ð�

public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) override;

public:
	PlayerIdleAction(Player* pOwner, float idleTime) : PlayerAction(pOwner, PLAYER_ACTIONID_IDLE, PLAYER_ANIMATIONID_IDLE), m_IdleTime(idleTime) { };
	virtual ~PlayerIdleAction() {};

};