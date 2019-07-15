#include "stdafx.h"

#include "Movement.h"

#include "ElectricBall.h"
 
ElectricBall::ElectricBall(const std::string & entityID)
	:SkillEffect(entityID, 5.f)
{ 
}

ElectricBall::~ElectricBall()
{
}
 