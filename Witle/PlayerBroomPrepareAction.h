#pragma once
#include "PlayerAction.h"

class MonsterMovement;
class PlayerActionMgr;

// 보스몬스터용 대기 액션
// 움직이지 않습니다.
// 만약 UpdateState 시 , 플레이어와 거리가 500 안으로 들어오면 플레이어에게 따라갑니다.
class PlayerBroomPrepareAction
	: public PlayerAction
{
	const float m_IdleTime{ 3.f }; //Idle 상태로 있는 시간

public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) override;

public:
	PlayerBroomPrepareAction(Player* pOwner, float idleTime) : PlayerAction(pOwner, PLAYER_ACTIONID_BROOMPREPARE, PLAYER_ANIMATIONID_BROOMPREPARE), m_IdleTime(idleTime) { };
	virtual ~PlayerBroomPrepareAction() {};

};