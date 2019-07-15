#pragma once 
#include "BossSampleAction_1.h"
#include "BossSampleAction_2.h"
#include "MonsterActionMgr.h"

class MonsterAction;
class MonsterMovement;
class Player;
 
// 보스 몬스터용 액션 매니저입니다.
// MonsterActionMgr를 상속받아 현재 액션상태를 관리합니다. 
class BossMonsterActionMgr
	: public MonsterActionMgr
{  
	// 현재 사용하는 액션(상태)들
	BossSampleAction_1 m_BossSampleAction_1;
	BossSampleAction_2 m_BossSampleAction_2;
	// 현재 사용하는 액션(상태)들

public:
	virtual void UpdateState(float fElpasedTime) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};
	
public:
	BossMonsterActionMgr(GameObject* pOwner) : 
		MonsterActionMgr(pOwner),
		m_BossSampleAction_1(pOwner),
		m_BossSampleAction_2(pOwner)
	{
	};
	virtual ~BossMonsterActionMgr() {};

	void ChangeStateToSample_1();
	void ChangeStateToSample_2();
};
