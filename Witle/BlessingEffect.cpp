#include "stdafx.h"

#include "GraphicsRootSignatureMgr.h"
#include "SphereMesh.h"
#include "MyBSphere.h"

#include "BlessingEffect.h"

#include "SoundManager.h"

void BlessingEffect::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	m_BlessingEffectMesh->Render(pd3dCommandList, isGBuffers);
}

void BlessingEffect::UpdateCollider()
{

}

void BlessingEffect::ReleaseMembers()
{
	SkillEffect::ReleaseMembers();

	m_BlessingEffectMesh->ReleaseObjects();
	delete m_BlessingEffectMesh;
	m_BlessingEffectMesh = nullptr;
}

void BlessingEffect::ReleaseMemberUploadBuffers()
{
	SkillEffect::ReleaseMemberUploadBuffers();

	if (m_BlessingEffectMesh) m_BlessingEffectMesh->ReleaseUploadBuffers();
}

BlessingEffect::BlessingEffect(const std::string & entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
	:SkillEffect(entityID, 5.f, ENUM_SKILLTYPE::SKILLTYPE_BUFF)
{
	// SoundManager::GetInstance()->Play(ENUM_SOUND::PLAYER_BLESSING_SOUND);

	m_BlessingEffectMesh = new SphereMesh(this, pd3dDevice, pd3dCommandList, 50, 50, 10, 10);
	 
}

BlessingEffect::~BlessingEffect()
{

}
