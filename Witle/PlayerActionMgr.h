#pragma once 
#include "ComponentBase.h"

class PlayerAction;
class PlayerMovement;
class Player;

class PlayerActionMgr
	: public ComponentBase
{
protected:
	// ���� ����ϰ� �ִ� �׼�
	PlayerAction*  m_CurrMonsterAction{ nullptr };

	// ������ ����ߴ� �׼�
	PlayerAction*  m_BeforeMonsterAction{ nullptr };
	 
public:
	virtual void UpdateState(float fElpasedTime) = 0;

protected:
	// �׼��� �����մϴ�.
	// ���� ���� �׼ǰ� ���� �׼��� �����ϴٸ� �������� �ʽ��ϴ�.
	void ChangeAction(PlayerAction* );

public:
	PlayerActionMgr(GameObject* pOwner) : ComponentBase(pOwner) { };
	virtual ~PlayerActionMgr() {};

	// Scene�� Update ���� ���� �ݵ�� ȣ���ؾ��մϴ�.
	// movement���Լ� ���ӵ��� ������ �����մϴ�.
	void UpdateVelocity(float fElpasedTime, PlayerMovement* movement);
};