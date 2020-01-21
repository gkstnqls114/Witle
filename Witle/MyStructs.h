#pragma once
#include "stdafx.h"

struct ROOTPARAMETER_INDEX
{
	UINT rpIndex;

	explicit ROOTPARAMETER_INDEX(UINT index)
	{
		rpIndex = index;
	}
};

struct VS_SRV_INSTANCEINFO
{
	XMFLOAT4X4 m_xmf4x4Transform;
};

struct MATERIAL
{
	XMFLOAT4						m_xmf4Ambient;
	XMFLOAT4						m_xmf4Diffuse;
	XMFLOAT4						m_xmf4Specular; //(r,g,b,a=power)
	XMFLOAT4						m_xmf4Emissive;
};
