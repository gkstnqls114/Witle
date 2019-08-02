#include "stdafx.h"
#include "LightningBall.h"
#include "LightningBallSkill.h"


LightningBallSkill::LightningBallSkill(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
	: BallSkill("LightningBallSkill", COOLTIME_LIGHTNINGBALL)
{
	m_skillEffect = new LightningBall("FireBallEffect", pd3dDevice, pd3dCommandList);
}

LightningBallSkill::~LightningBallSkill()
{
}
