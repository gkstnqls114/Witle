#pragma once 
#include "BossSampleAction_1.h"
#include "BossSampleAction_2.h"
#include "MonsterActionMgr.h"

class MonsterAction;
class MonsterMovement;
class BossSampleAction_1;
class BossSampleAction_2;
class Player;
 
class BossMonsterActionMgr
	: public MonsterActionMgr
{  
	BossSampleAction_1 m_BossSampleAction_1;
	BossSampleAction_2 m_BossSampleAction_2;

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
