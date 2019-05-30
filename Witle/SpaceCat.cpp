#include "stdafx.h"

// For Monster ///////////////
#include "MonsterMovement.h"
#include "MonsterStatus.h"
#include "RecognitionRange.h"
#include "PlayerManager.h"
// For Monster ///////////////

#include "MyDescriptorHeap.h"
#include "Object.h"
#include "MyBOBox.h"
#include "Texture.h"
#include "SpaceCat.h"

SpaceCat::SpaceCat(const std::string & entityID, const XMFLOAT3& SpawnPoint, 
	ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature)
	: Monster(entityID, SpawnPoint, pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature)
{ 
	m_pHaep = new MyDescriptorHeap();
	m_pHaep->CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1, 0);

	m_pTexture = new Texture(1, RESOURCE_TEXTURE2D);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/M_Cat_DA.dds", 0);
	
	m_pHaep->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture, ROOTPARAMETER_TEXTURE, false, 0);
	
	ANIMATION_INFO infos[SPACECAT_ANIMATIONE];
	infos[0] = SPACECAT_IDLE;
	infos[1] = SPACECAT_MOVE;
	infos[2] = SPACECAT_ATTACK;
	infos[3] = SPACECAT_DEAD;
	infos[4] = SPACECAT_HIT;
	 
	m_MonsterModel = LoadObject::LoadGeometryAndAnimationFromFile_forMonster(
		pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/SpaceCat.bin", NULL,
		SPACECAT_ANIMATIONE, infos);
	m_pLoadObject = m_MonsterModel->m_pModelRootObject;

	m_pLoadObject->m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, 1, m_MonsterModel);
	m_pLoadObject->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);

	m_Transform.SetPosition(SpawnPoint); 


	XMFLOAT3 extents{ 100.f, 50.f, 100.f };
	m_pMyBOBox = new MyBOBox(this, pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 75.F, 0.F }, extents);

}

SpaceCat::~SpaceCat()
{
}

void SpaceCat::Update(float fElapsedTime)
{
	// �̵����� ����Ѵ�. 
	m_MonsterMovement->Update(fElapsedTime);

	// �̵�����ŭ �����δ�. 
	Move(Vector3::ScalarProduct(m_MonsterMovement->m_xmf3Velocity, fElapsedTime, false));

}
 
void SpaceCat::UpdateState(float fElapsedTime)
{
	m_MonsterMovement->UpdateState(fElapsedTime);
}

void SpaceCat::Animate(float fElapsedTime)
{
	Monster::Animate(fElapsedTime);
	  
	m_pMyBOBox->SetPosition(
		XMFLOAT3(
			m_Transform.GetPosition().x,
			150.f,
			m_Transform.GetPosition().z)
	);
}
