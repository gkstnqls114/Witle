#include "stdafx.h"

#include "GraphicsRootSignatureMgr.h"
#include "EffectMesh.h"
#include "MyBSphere.h"

#include "HealingEffect.h"

#include "SoundManager.h"

void HealingEffect::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	m_HealingEffectMesh->Render(pd3dCommandList, isGBuffers);
}

void HealingEffect::UpdateCollider()
{

}

void HealingEffect::ReleaseMembers()
{
	SkillEffect::ReleaseMembers();

	m_HealingEffectMesh->ReleaseObjects();
	delete m_HealingEffectMesh;
	m_HealingEffectMesh = nullptr;
}

void HealingEffect::ReleaseMemberUploadBuffers()
{
	SkillEffect::ReleaseMemberUploadBuffers();

	if (m_HealingEffectMesh) m_HealingEffectMesh->ReleaseUploadBuffers();
}

HealingEffect::HealingEffect(const std::string & entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
	:SkillEffect(entityID, 5.f, ENUM_SKILLTYPE::SKILLTYPE_RECOVERY)
{
	m_HealingEffectMesh = new EffectMesh(this, pd3dDevice, pd3dCommandList, 30);	 

	SoundManager::GetInstance()->Play(ENUM_SOUND::PLAYER_HEALING_SOUND);
}

HealingEffect::~HealingEffect()
{

}
