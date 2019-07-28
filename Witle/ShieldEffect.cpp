#include "stdafx.h"

#include "GraphicsRootSignatureMgr.h"
#include "SphereMesh.h"
#include "MyBSphere.h"

#include "ShieldEffect.h"

void ShieldEffect::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	m_ShieldEffectMesh->Render(pd3dCommandList, isGBuffers);
}

void ShieldEffect::UpdateCollider()
{
	static_cast<MyBSphere*>(m_MyCollider)->GetBSphere()->Center = m_Transform.GetPosition();
}

void ShieldEffect::ReleaseMembers()
{
	SkillEffect::ReleaseMembers();

	m_ShieldEffectMesh->ReleaseObjects();
	delete m_ShieldEffectMesh;
	m_ShieldEffectMesh = nullptr;
}

void ShieldEffect::ReleaseMemberUploadBuffers()
{
	SkillEffect::ReleaseMemberUploadBuffers();

	if (m_ShieldEffectMesh) m_ShieldEffectMesh->ReleaseUploadBuffers();
}

ShieldEffect::ShieldEffect(const std::string & entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
	:SkillEffect(entityID, 5.f)
{
	m_ShieldEffectMesh = new SphereMesh(this, pd3dDevice, pd3dCommandList, 50, 50, 10, 10);
	 
}

ShieldEffect::~ShieldEffect()
{

}
