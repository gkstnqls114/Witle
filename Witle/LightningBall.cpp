#include "stdafx.h"

#include "Movement.h"

#include "LightningBall.h"

#include "SoundManager.h"

LightningBall::LightningBall(const std::string & entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
	:BallEffect(entityID, pd3dDevice, pd3dCommandList, ENUM_BALLEFFECT::BALLEFFECT_LIGHTNING)
{ 
	SoundManager::GetInstance()->Play(ENUM_SOUND::PLAYER_LIGHT_SOUND);
}

LightningBall::~LightningBall()
{
}
 