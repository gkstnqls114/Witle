#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// 기본 몬스터용 대기 액션
// 움직이지 않습니다. 일정 시간이 지나면 Move로 전환합니다.
// 플레이어와 일정 거리안에 일정시간동안 있다면 Chase로 전환합니다.
class IdleAction
	: public GeneralMonsterAction
{
	const float m_IdleTime{ 3.f }; //Idle 상태로 있는 시간
	 
public:
	// Update 수행 이전 반드시 호출
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, GeneralMonsterActionMgr* actionMgr) override;

public:
	IdleAction(GameObject* pOwner, float idleTime) : GeneralMonsterAction(pOwner, GENERALMONSTER_ACTIONID_IDLE, GENERALMONSTER_ANIMATIONID_IDLE), m_IdleTime(idleTime) { };
	virtual ~IdleAction() {};

};