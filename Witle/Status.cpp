#include "stdafx.h" 
#include "GameObject.h"
#include "Status.h"
  
Status::Status(GameObject * pOwner )
	: ComponentBase(pOwner)
{ 

}

Status::~Status()
{

}

void Status::Damage(int damage)
{
	m_HP -= damage;
	std::cout << m_pOwner->GetName() << " " << m_HP << std::endl;
}
 