#include "stdafx.h"
#include "IceBall.h"
#include "IceBallSkill.h"

 
IceBallSkill::IceBallSkill(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
	: BallSkill("IceBall", ENUM_SELECTABLESKILL::SELECTABLESKILL_ICEBALL, COOLTIME_ICEBALL)
{
	m_skillEffect = new IceBall("IceBallffect", pd3dDevice, pd3dCommandList);
}

IceBallSkill::~IceBallSkill()
{
}
