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
	const float m_HitFullTime{ ANIMATION_HIT.EndTime - ANIMATION_HIT.StartTime }; //Hit ���·� �ִ� �ð�
	float m_HitElapsedTime{ 0.f };

public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override { m_HitElapsedTime = 0.f; };
	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) override;

public:
	PlayerHitAction(Player* pOwner) : PlayerAction(pOwner, PLAYER_ACTIONID_HIT, PLAYER_ANIMATIONID_HIT), m_HitFullTime(ANIMATION_HIT.EndTime - ANIMATION_HIT.StartTime) { };
	virtual ~PlayerHitAction() {};

};