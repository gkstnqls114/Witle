#pragma once
#include "Action.h"

class PlayerMovement;
class PlayerActionMgr;

// 몬스터 액션의 부모 클래스.
class PlayerAction
	: public Action
{
public:
	const ENUM_PLAYER_ACTIONID m_ActionID{ PLAYER_ACTIONID_ERROR };

public:
	// 초기화 함수.
	virtual void Init() = 0;

	// 가속도를 설정.
	// Scene에서 Update 수행 이전 반드시 호출해야합니다.
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
