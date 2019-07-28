#include "stdafx.h"

#include "GraphicsRootSignatureMgr.h"
#include "SphereMesh.h"
#include "MyBSphere.h"

#include "HealingEffect.h"

void HealingEffect::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	m_HealingEffectMesh->Render(pd3dCommandList, isGBuffers);
}

void HealingEffect::UpdateCollider()
{
	static_cast<MyBSphere*>(m_MyCollider)->GetBSphere()->Center = m_Transform.GetPosition();
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
	m_HealingEffectMesh = new SphereMesh(this, pd3dDevice, pd3dCommandList, 50, 50, 10, 10);
	 
}

HealingEffect::~HealingEffect()
{

}
