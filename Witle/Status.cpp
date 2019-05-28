#include "stdafx.h" 
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
	SetAnimationStateToHit(); 
	SubstractHP(damage);
	std::cout << m_pOwner->GetName() << " " << m_HP << std::endl;
}
 