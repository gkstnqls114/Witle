#include "stdafx.h"

#include "GraphicsRootSignatureMgr.h"
#include "SphereMesh.h"
#include "MyBSphere.h"

#include "BallEffect.h"

void BallEffect::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	m_BallEffectMesh->Render(pd3dCommandList, isGBuffers);
}

void BallEffect::UpdateCollider()
{
	static_cast<MyBSphere*>(m_MyCollider)->GetBSphere()->Center = m_Transform.GetPosition();
}

void BallEffect::ReleaseMembers()
{
	SkillEffect::ReleaseMembers();

	m_BallEffectMesh->ReleaseObjects();
	delete m_BallEffectMesh;
	m_BallEffectMesh = nullptr;
}

void BallEffect::ReleaseMemberUploadBuffers()
{
	SkillEffect::ReleaseMemberUploadBuffers();

	if (m_BallEffectMesh) m_BallEffectMesh->ReleaseUploadBuffers();
}

BallEffect::BallEffect(const std::string & entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ENUM_BALLEFFECT type)
	:SkillEffect(entityID, 5.f, ENUM_SKILLTYPE::SKILLTYPE_ATTACK)
{
	m_BallEffectMesh = new SphereMesh(this, pd3dDevice, pd3dCommandList, 50, 50, 10, 10);

	switch (type)
	{
	case BALLEFFECT_FIRE:
		m_BallEffectMesh->CreateTexture(pd3dDevice, pd3dCommandList, L"Image/Red.dds");
		break;
	case BALLEFFECT_ICE:
		m_BallEffectMesh->CreateTexture(pd3dDevice, pd3dCommandList, L"Image/Blue.dds");
		break;
	case BALLEFFECT_LIGHTNING:
		m_BallEffectMesh->CreateTexture(pd3dDevice, pd3dCommandList, L"Image/Yellow.dds");
		break;
	default:
		break;
	}

	m_MyCollider = new MyBSphere(this, pd3dDevice, pd3dCommandList, 50);
}

BallEffect::~BallEffect()
{

}
