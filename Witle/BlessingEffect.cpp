#include "stdafx.h"

#include "GraphicsRootSignatureMgr.h"
#include "SphereMesh.h"
#include "MyBSphere.h"

#include "BlessingEffect.h"

#include "SoundManager.h"

void BlessingEffect::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{ 
}

void BlessingEffect::UpdateCollider()
{

}

void BlessingEffect::ReleaseMembers()
{
	SkillEffect::ReleaseMembers(); 
}

void BlessingEffect::ReleaseMemberUploadBuffers()
{
	SkillEffect::ReleaseMemberUploadBuffers();
}

BlessingEffect::BlessingEffect(const std::string & entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
	:SkillEffect(entityID, 5.f, ENUM_SKILLTYPE::SKILLTYPE_BUFF)
{
	// SoundManager::GetInstance()->Play(ENUM_SOUND::PLAYER_BLESSING_SOUND);
	 
}

BlessingEffect::~BlessingEffect()
{

}
