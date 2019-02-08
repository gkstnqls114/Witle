#include "stdafx.h"
#include "Light.h"

Light::Light(GameObject * pOwner) 
	: ResourceComponentBase(pOwner)
{
	m_FamilyID.InitLight();
}

Light::~Light()
{

}
