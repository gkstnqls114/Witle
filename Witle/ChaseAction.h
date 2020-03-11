#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// 기본 몬스터용 플레이어 쫓아다니기 액션
// 플레이어를 쫓아다닙니다. 
class ChaseAction
	: public GeneralMonsterAction
{
	float m_CognizeTime{ 0.f }; // 인식범위 벗어나는 시간 
	const float m_MaxCognizeTime{ 3.f };

public:
	// Update 수행 이전 반드시 호출
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, GeneralMonsterActionMgr* actionMgr) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	ChaseAction(GameObject* pOwner) : GeneralMonsterAction(pOwner, GENERALMONSTER_ACTIONID_CHASE, SPACECAT_MOVE.ID) {};
	virtual ~ChaseAction() {};
};