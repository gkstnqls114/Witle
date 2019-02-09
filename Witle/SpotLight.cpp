#include "stdafx.h"
#include "SpotLight.h"

SpotLight::SpotLight(GameObject * pOwner)
	: Light(pOwner)
{
	m_Light.nType = LIGHT_TYPE::SPOT_LIGHT;
}

SpotLight::~SpotLight()
{
}

void SpotLight::Update(float fTimeElapsed)
{
}
