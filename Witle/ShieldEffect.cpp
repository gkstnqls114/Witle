#include "stdafx.h"

#include "GraphicsRootSignatureMgr.h"
#include "Object.h"
#include "MyBSphere.h"
#include "PlayerManager.h"
#include "Player.h"
#include "SphereMesh.h"
#include "Texture.h"
#include "TextureStorage.h"

#include "ShieldEffect.h"

#include "SoundManager.h"

void ShieldEffect::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	m_ShiedlTexture->UpdateShaderVariable(pd3dCommandList, 0);
	 
	m_ShieldEffectMesh->Render(pd3dCommandList, isGBuffers);
}

void ShieldEffect::UpdateCollider()
{ 
}

void ShieldEffect::ReleaseMembers()
{
	SkillEffect::ReleaseMembers();

	if (m_ShieldEffectObject)
	{ 
	m_ShieldEffectObject->ReleaseObjects();
	delete m_ShieldEffectObject;
	m_ShieldEffectObject = nullptr;
	}

	m_ShieldEffectMesh->ReleaseObjects();
	delete m_ShieldEffectMesh;
	m_ShieldEffectMesh = nullptr;

}

void ShieldEffect::ReleaseMemberUploadBuffers()
{
	SkillEffect::ReleaseMemberUploadBuffers();

	if (m_ShieldEffectObject) m_ShieldEffectObject->ReleaseUploadBuffers();
	if (m_ShieldEffectMesh) m_ShieldEffectMesh->ReleaseUploadBuffers();

}

ShieldEffect::ShieldEffect(const std::string & entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
	:SkillEffect(entityID, 5.f, ENUM_SKILLTYPE::SKILLTYPE_BUFF)
{
	// SoundManager::GetInstance()->Play(ENUM_SOUND::PLAYER_SHIELD_SOUND);

	m_ShieldEffectMesh = new SphereMesh(this, pd3dDevice, pd3dCommandList, true, 150, 150, 10, 10);
	// m_ShieldEffectMesh->CreateTexture(pd3dDevice, pd3dCommandList, L"Image/Shield.dds"); 

		m_ShieldEffectModelInfo = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature(), "Model/Shield.bin", NULL);
	m_ShieldEffectObject = m_ShieldEffectModelInfo->m_pModelRootObject;

	m_ShiedlTexture = TextureStorage::GetInstance()->GetTexture("Shield");
}


ShieldEffect::~ShieldEffect()
{

}

void ShieldEffect::Update(float fElapsedTime)
{
	m_Transform.SetLook(Vector3::ScalarProduct(PlayerManager::GetMainPlayer()->GetTransform().GetLook(), -1)); 
	m_Transform.SetUp(PlayerManager::GetMainPlayer()->GetTransform().GetUp());
	m_Transform.SetRight(PlayerManager::GetMainPlayer()->GetTransform().GetRight());
	m_Transform.SetPosition(Vector3::Add(PlayerManager::GetMainPlayer()->GetTransform().GetPosition(), Vector3::ScalarProduct(XMFLOAT3(0, 1, 0), 75.f, false)));
	m_Transform.Update(0.f);

	m_ShieldEffectObject->m_xmf4x4ToParent = m_Transform.GetWorldMatrix();
	m_ShieldEffectObject->Animate(fElapsedTime);
}
