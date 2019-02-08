#include "stdafx.h"
#include "Light.h"

Light::Light(GameObject * pOwner) 
	: ResourceComponentBase(pOwner)
{
	m_FamillyID = "Light";
}

Light::~Light()
{

}
