#include "stdafx.h"

#include "Movement.h"

#include "IceBall.h"

#include "SoundManager.h"
 
IceBall::IceBall(const std::string & entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
	:BallEffect(entityID, pd3dDevice, pd3dCommandList, ENUM_BALLEFFECT::BALLEFFECT_ICE)
{ 
	SoundManager::GetInstance()->Play(ENUM_SOUND::PLAYER_ICE_SOUND);
}

IceBall::~IceBall()
{
}
 