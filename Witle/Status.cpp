#include "stdafx.h" 
#include "Monster.h"
#include "GeneralMonsterActionMgr.h"
#include "MonsterMovement.h"
#include "GameObject.h"
#include "Status.h"
  
void Status::SubstractHP(int damage)
{ 
	m_Guage -= damage;
}

Status::Status(GameObject * pOwner )
	: ComponentBase(pOwner)
{ 

}

Status::~Status()
{

} 
 