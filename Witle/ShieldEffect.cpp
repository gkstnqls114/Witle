#include "stdafx.h"

#include "GraphicsRootSignatureMgr.h"
#include "SphereMesh.h"
#include "MyBSphere.h"
#include "PlayerManager.h"
#include "Player.h"

#include "ShieldEffect.h"

#include "SoundManager.h"

void ShieldEffect::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	m_ShieldEffectMesh->Render(pd3dCommandList, isGBuffers);
}

void ShieldEffect::UpdateCollider()
{

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
	:SkillEffect(entityID, 5.f, ENUM_SKILLTYPE::SKILLTYPE_BUFF)
{
	SoundManager::GetInstance()->Play(ENUM_SOUND::PLAYER_SHIELD_SOUND);

	m_ShieldEffectMesh = new SphereMesh(this, pd3dDevice, pd3dCommandList, true, 150, 150, 10, 10);
	// m_ShieldEffectMesh->CreateTexture(pd3dDevice, pd3dCommandList, L"Image/Shield.dds");
}


ShieldEffect::~ShieldEffect()
{

}

void ShieldEffect::Update(float)
{
	m_Transform.SetIdentity();
	m_Transform.SetPosition(Vector3::Add(PlayerManager::GetMainPlayer()->GetTransform().GetPosition(), Vector3::ScalarProduct(XMFLOAT3(0, 1, 0), 75.f, false)));
	m_Transform.Update(0.f);
}
