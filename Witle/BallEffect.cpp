#include "stdafx.h"

#include "GraphicsRootSignatureMgr.h"
#include "SphereMesh.h"
#include "MyBSphere.h"
#include "MyDescriptorHeap.h"
#include "Texture.h"

#include "BallEffect.h"

void BallEffect::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	m_Heap->UpdateShaderVariable(pd3dCommandList);

	m_Texture->UpdateShaderVariable(pd3dCommandList, 1);
	m_InBallEffectMesh->Render(pd3dCommandList, isGBuffers);

	m_Texture->UpdateShaderVariable(pd3dCommandList, 0);
	m_BallEffectMesh->Render(pd3dCommandList, isGBuffers);
}

void BallEffect::UpdateCollider()
{
	static_cast<MyBSphere*>(m_MyCollider)->GetBSphere()->Center = m_Transform.GetPosition();
}

void BallEffect::ReleaseMembers()
{
	SkillEffect::ReleaseMembers();

	if (m_BallEffectMesh)
	{
		m_BallEffectMesh->ReleaseObjects();
		delete m_BallEffectMesh;
		m_BallEffectMesh = nullptr;
	}

	if (m_InBallEffectMesh)
	{
		m_InBallEffectMesh->ReleaseObjects();
		delete m_InBallEffectMesh;
		m_InBallEffectMesh = nullptr;
	}
}

void BallEffect::ReleaseMemberUploadBuffers()
{
	SkillEffect::ReleaseMemberUploadBuffers();

	if (m_BallEffectMesh) m_BallEffectMesh->ReleaseUploadBuffers();
	if (m_InBallEffectMesh) m_InBallEffectMesh->ReleaseUploadBuffers();
}

BallEffect::BallEffect(const std::string & entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ENUM_BALLEFFECT type)
	:SkillEffect(entityID, 5.f, ENUM_SKILLTYPE::SKILLTYPE_ATTACK)
{
	m_BallEffectMesh = new SphereMesh(this, pd3dDevice, pd3dCommandList, true, 50, 50, 10, 10);
	m_InBallEffectMesh = new SphereMesh(this, pd3dDevice, pd3dCommandList, true, 20, 20, 10, 10);
	
	m_Heap = new MyDescriptorHeap();
	m_Heap->CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 2, 0, ENUM_SCENE::SCENE_GAME);
	m_Texture = new Texture(2);

	switch (type)
	{
	case BALLEFFECT_FIRE:
		m_Texture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Red.dds", 0);
		m_Texture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Yellow.dds", 1);
		break;
	case BALLEFFECT_ICE:
		m_Texture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Blue.dds", 0); 
		m_Texture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Black.dds", 1);
		break;
	case BALLEFFECT_LIGHTNING:
		m_Texture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Yellow.dds", 0);
		m_Texture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Black.dds", 1);
		break;
	default:
		break;
	}
	m_Heap->CreateShaderResourceViews(pd3dDevice, m_Texture, ROOTPARAMETER_TEXTURE, false);

	m_MyCollider = new MyBSphere(this, pd3dDevice, pd3dCommandList, 50);
}

BallEffect::~BallEffect()
{

}
