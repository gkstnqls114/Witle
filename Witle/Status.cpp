#include "stdafx.h" 
#include "Monster.h"
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
	static_cast<Monster*>(m_pOwner)->GetMovement()->GetMonsterActionMgr()->ChangeStateToHit();
	SubstractHP(damage);
	std::cout << m_pOwner->GetName() << " " << m_HP << std::endl;
}
 