#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// �������Ϳ� ���� �׼�
// �������� �ʽ��ϴ�.
// ���� �� �׼��� ������� �ʽ��ϴ�.
class BossBuf
	: public BossMonsterAction
{ 
public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	BossBuf(GameObject* pOwner ) : BossMonsterAction(pOwner, BOSSMONSTER_ACTIONID_BUF, BOSSMONSTER_ANIMATIONID_BUF)  { };
	virtual ~BossBuf() {};

};