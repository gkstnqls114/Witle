#include "stdafx.h"
#include "FireBall.h"
#include "FireBallSkill.h"

 
FireBallSkill::FireBallSkill(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
	:BallSkill("FrieBall", COOLTIME_FIREBALL)
{
	m_skillEffect = new FireBall("FireBallEffect", pd3dDevice, pd3dCommandList);
}

FireBallSkill::~FireBallSkill()
{
}
