#include "stdafx.h"

#include "Movement.h"

#include "IceBall.h"
 
IceBall::IceBall(const std::string & entityID)
	:SkillEffect(entityID, 5.f)
{ 
}

IceBall::~IceBall()
{
}
 