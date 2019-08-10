#pragma once
#include "MonsterAction.h"
#include "GameScene.h"

class MonsterMovement;

// MonsterMovement�� ���ڷ� �޾� ���ӵ��� ó�����ִ� Ŭ����.
// �ʵ带 ������ �������� ���ƴٴѴ�.
class BossChaseAction
	: public BossMonsterAction
{
private:
	// ��� ������Ʈ�� �Ϸ�ȴٸ� return true�� �մϴ�.
	bool UpdateFirstPhase(float fElpasedTime, BossMonsterActionMgr* actionMgr);
	bool UpdateSecondPhase(float fElpasedTime, BossMonsterActionMgr* actionMgr);
	bool UpdateLastPhase(float fElpasedTime, BossMonsterActionMgr* actionMgr);

public:
	// Update ���� ���� �ݵ�� ȣ��
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	BossChaseAction(GameObject* pOwner) : BossMonsterAction(pOwner) {};
	virtual ~BossChaseAction() {};
};