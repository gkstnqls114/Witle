#include "stdafx.h"
#include "IceBall.h"
#include "IceBallSkill.h"

 
IceBallSkill::IceBallSkill(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_skillEffect = new IceBall("FireBallEffect", pd3dDevice, pd3dCommandList);
}

IceBallSkill::~IceBallSkill()
{
}
