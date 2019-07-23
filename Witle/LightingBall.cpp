#include "stdafx.h"

#include "Movement.h"

#include "LightingBall.h"
 
LightingBall::LightingBall(const std::string & entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
	:BallEffect(entityID, pd3dDevice, pd3dCommandList, ENUM_BALLEFFECT::BALLEFFECT_LIGHTING)
{ 
}

LightingBall::~LightingBall()
{
}
 