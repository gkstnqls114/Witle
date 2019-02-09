#include "stdafx.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(GameObject * pOwner)
	: Light(pOwner)
{
	m_Light.nType = LIGHT_TYPE::DIRECTIONAL_LIGHT;
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::Update(float fTimeElapsed)
{
}
