#pragma once
#include "PlayerAction.h"

class MonsterMovement;
class PlayerActionMgr;

// �������Ϳ� ��� �׼�
// �������� �ʽ��ϴ�.
// ���� UpdateState �� , �÷��̾�� �Ÿ��� 500 ������ ������ �÷��̾�� ���󰩴ϴ�.
class PlayerBroomPrepareAction
	: public PlayerAction
{
	const float m_IdleTime{ 3.f }; //Idle ���·� �ִ� �ð�

public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) override;

public:
	PlayerBroomPrepareAction(GameObject* pOwner, float idleTime) : PlayerAction(pOwner, PLAYER_ACTIONID_BROOMPREPARE, PLAYER_ANIMATIONID_BROOMPREPARE), m_IdleTime(idleTime) { };
	virtual ~PlayerBroomPrepareAction() {};

};