#pragma once
#include "Action.h"

class PlayerMovement;
class PlayerActionMgr;

// ���� �׼��� �θ� Ŭ����.
class PlayerAction
	: public Action
{
public:
	const ENUM_PLAYER_ACTIONID m_ActionID{ PLAYER_ACTIONID_ERROR };

public:
	// �ʱ�ȭ �Լ�.
	virtual void Init() = 0;

	// ���ӵ��� ����.
	// Scene���� Update ���� ���� �ݵ�� ȣ���ؾ��մϴ�.
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override = 0;
	  
	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) = 0;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	PlayerAction(GameObject* pOwner, ENUM_PLAYER_ACTIONID actionID, ENUM_ANIMATIONID animeID)
		: Action(pOwner, animeID), m_ActionID(actionID) {};
	virtual ~PlayerAction() {};

};
