#include "stdafx.h" 
#include "Monster.h"
#include "GeneralMonsterActionMgr.h"
#include "MonsterMovement.h"
#include "GameObject.h"
#include "Status.h"
  
void Status::SubstractHP(float damage)
{ 
	m_Guage -= damage;
	if (m_Guage < 0.f) { m_Guage = 0.f; };
}

void Status::AddHP(float damage)
{
	m_Guage += damage;
	if (m_Guage > m_MAXGuage) { m_Guage = m_MAXGuage; };
}

Status::Status(GameObject * pOwner )
	: ComponentBase(pOwner)
{ 

}

Status::~Status()
{

} 
 