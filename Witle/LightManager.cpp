#include "stdafx.h"
#include "LightManager.h"

LIGHT					LightManager::m_pLights[MAX_LIGHTS];
XMFLOAT4				LightManager::m_xmf4GlobalAmbient;

LightManager::LightManager()
{

}


LightManager::~LightManager()
{
}
