#pragma once
#include "stdafx.h"

struct LIGHT
{
	XMFLOAT4				Ambient;
	XMFLOAT4				Diffuse;
	XMFLOAT4				Specular;
	XMFLOAT3				Position;
	float 					fFalloff;
	XMFLOAT3				Direction;
	float 					fTheta; //cos(m_fTheta)
	XMFLOAT3				Attenuation;
	float					fPhi; //cos(m_fPhi)
	bool					bEnable;
	int						nType;
	float					fRange;
	float					padding;
};


struct LIGHTS
{
	LIGHT					m_pLights[MAX_LIGHTS];
	XMFLOAT4				m_xmf4GlobalAmbient;
};

struct MATERIAL
{
	XMFLOAT4						m_xmf4Ambient;
	XMFLOAT4						m_xmf4Diffuse;
	XMFLOAT4						m_xmf4Specular; //(r,g,b,a=power)
	XMFLOAT4						m_xmf4Emissive;
};

struct MATERIALS
{
	MATERIAL				m_pReflections[MAX_MATERIALS];
};
