#pragma once
#include "PlayerAction.h"

class MonsterMovement;
class PlayerActionMgr;

// �������Ϳ� ��� �׼�
// �������� �ʽ��ϴ�.
// ���� UpdateState �� , �÷��̾�� �Ÿ��� 500 ������ ������ �÷��̾�� ���󰩴ϴ�.
class PlayerHitAction
	: public PlayerAction
{
	const float m_IdleTime{ 3.f }; //Idle ���·� �ִ� �ð�

public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) override;

public:
	PlayerHitAction(Player* pOwner, float idleTime) : PlayerAction(pOwner, PLAYER_ACTIONID_HIT, PLAYER_ANIMATIONID_HIT), m_IdleTime(idleTime) { };
	virtual ~PlayerHitAction() {};

};