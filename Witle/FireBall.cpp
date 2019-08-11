#include "stdafx.h"

#include "GraphicsRootSignatureMgr.h"
#include "SphereMesh.h"
#include "MyBSphere.h"

#include "FireBall.h"

#include "SoundManager.h"
  
void FireBall::ReleaseMembers()
{
	BallEffect::ReleaseMembers();
}

void FireBall::ReleaseMemberUploadBuffers()
{
	SoundManager::GetInstance()->Play(ENUM_SOUND::PLAYER_FIRE_SOUND);

	BallEffect::ReleaseMemberUploadBuffers();
}

FireBall::FireBall(const std::string & entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
	:BallEffect(entityID, pd3dDevice, pd3dCommandList, ENUM_BALLEFFECT::BALLEFFECT_FIRE)
{

}

FireBall::~FireBall()
{

}
 