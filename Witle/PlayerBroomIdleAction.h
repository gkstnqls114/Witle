#pragma once
#include "PlayerAction.h"

class MonsterMovement;
class PlayerActionMgr;

// �������Ϳ� ��� �׼�
// �������� �ʽ��ϴ�.
// ���� UpdateState �� , �÷��̾�� �Ÿ��� 500 ������ ������ �÷��̾�� ���󰩴ϴ�.
class PlayerBroomIdleAction
	: public PlayerAction
{
	const float m_IdleTime{ 3.f }; //Idle ���·� �ִ� �ð�

public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) override;

public:
	PlayerBroomIdleAction(Player* pOwner, float idleTime) : PlayerAction(pOwner, PLAYER_ACTIONID_BROOMIDLE, PLAYER_ANIMATIONID_BROOMIDLE), m_IdleTime(idleTime) { };
	virtual ~PlayerBroomIdleAction() {};

};