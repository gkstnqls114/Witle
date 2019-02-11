#pragma once
#include "Light.h"

struct LIGHTS
{
	LIGHT					m_pLights[MAX_LIGHTS];
	XMFLOAT4				m_xmf4GlobalAmbient;
};

class LightManager
{
public:

public:
	static LIGHTS*					m_pLights;

private:

public:
	LightManager();
	~LightManager();


};

