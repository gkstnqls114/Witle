#include "stdafx.h"

#include "GraphicsRootSignatureMgr.h"
#include "SphereMesh.h"
#include "MyBSphere.h"

#include "BlessingEffect.h"

void BlessingEffect::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	m_BlessingEffectMesh->Render(pd3dCommandList, isGBuffers);
}

void BlessingEffect::UpdateCollider()
{
	static_cast<MyBSphere*>(m_MyCollider)->GetBSphere()->Center = m_Transform.GetPosition();
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
	:SkillEffect(entityID, 5.f)
{
	m_BlessingEffectMesh = new SphereMesh(this, pd3dDevice, pd3dCommandList, 50, 50, 10, 10);
	 
}

BlessingEffect::~BlessingEffect()
{

}
