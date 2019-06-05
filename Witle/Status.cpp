#include "stdafx.h" 
#include "Monster.h"
#include "GeneralMonsterActionMgr.h"
#include "MonsterMovement.h"
#include "GameObject.h"
#include "Status.h"
  
void Status::SubstractHP(int damage)
{ 
	m_HP -= damage;
}

Status::Status(GameObject * pOwner )
	: ComponentBase(pOwner)
{ 

}

Status::~Status()
{

}

void Status::Damage(int damage, int state)
{ 
	GeneralMonsterActionMgr* pMonsterActionMgr = static_cast<Monster*>(m_pOwner)->GetMovement()->GetMonsterActionMgr();
	if (pMonsterActionMgr->isDead()) return; // 만약에 죽은 경우 리턴한다.

	pMonsterActionMgr->ChangeStateToHit();
	SubstractHP(damage);
}
 