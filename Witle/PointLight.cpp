#include "stdafx.h"
#include "PointLight.h"

PointLight::PointLight(GameObject * pOwner)
	: Light(pOwner)
{
	m_Light.nType = LIGHT_TYPE::POINT_LIGHT;
}

PointLight::~PointLight()
{
}

void PointLight::Update(float fTimeElapsed)
{
}
