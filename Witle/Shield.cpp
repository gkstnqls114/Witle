#include "stdafx.h"
#include "ShieldEffect.h"
#include "Shield.h"


Shield::Shield(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_skillEffect = new ShieldEffect("ShieldEffect", pd3dDevice, pd3dCommandList);
}


Shield::~Shield()
{
}
