#pragma once
#include "PlayerAction.h"

class MonsterMovement;
class PlayerActionMgr;

// �÷��̾� ���ڷ� �̵�(+z��/-z��) �׼�  
// ���� �ش� Ŭ������  UpdateState, UpdateVelocity �Լ��� �ƴ�
// PlayerActionMgr Ŭ������ �Լ��� UpdateState, UpdateVelocity ���� ���ӵ��� ���� ��ȯ�� �ϰ��ֽ��ϴ�.
class PlayerBroomForwardAction
	: public PlayerAction
{
	const float m_IdleTime{ 3.f }; //Idle ���·� �ִ� �ð�

public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) override;

public:
	PlayerBroomForwardAction(Player* pOwner, float idleTime) : PlayerAction(pOwner, PLAYER_ACTIONID_BROOMFORWARD, PLAYER_ANIMATIONID_BROOMFORWARD), m_IdleTime(idleTime) { };
	virtual ~PlayerBroomForwardAction() {};

};