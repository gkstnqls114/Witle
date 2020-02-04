#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// 기본 몬스터용 플레이어 찾기 액션
// 현재 사용하지 않습니다.
class SearchAction
	: public GeneralMonsterAction
{
public:
	// Update 수행 이전 반드시 호출
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, GeneralMonsterActionMgr* actionMgr) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	SearchAction(GameObject* pOwner) : GeneralMonsterAction(pOwner, GENERALMONSTER_ACTIONID_SEARCH, GENERALMONSTER_ANIMATIONID_MOVE) {};
	virtual ~SearchAction() {};

};