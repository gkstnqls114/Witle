#include "stdafx.h"
#include "BlessingEffect.h"
#include "Blessing.h"


void Blessing::Active()
{
	
}

Blessing::Blessing(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_skillEffect = new BlessingEffect("BlessingEffect", pd3dDevice, pd3dCommandList);
}


Blessing::~Blessing()
{
}
