#pragma once
#include "Light.h"

class LightManager
{
public:

public:
	static LIGHT					m_pLights[MAX_LIGHTS];
	static XMFLOAT4				m_xmf4GlobalAmbient;

private:

public:
	LightManager();
	~LightManager();


};

