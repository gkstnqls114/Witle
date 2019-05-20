#include "stdafx.h"
#include "MyDescriptorHeap.h"
#include "Object.h"
#include "MonsterMovement.h"
#include "MonsterStatus.h"
#include "MyBOBox.h"
#include "Texture.h"
#include "Slime.h"

Slime::Slime(const std::string & entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature)
	: Monster(entityID, pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature)
{ 
	m_pHaep = new MyDescriptorHeap();
	m_pHaep->CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1, 0);

	m_pTexture = new Texture(1, RESOURCE_TEXTURE2D);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/Character_cloth_D.dds", 0);
	
	m_pHaep->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture, ROOTPARAMETER_TEXTURE, false, 0);
	
	m_MonsterModel = LoadObject::LoadGeometryAndAnimationFromFile_forPlayer(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Character.bin", NULL);
	m_pLoadObject = m_MonsterModel->m_pModelRootObject;

	m_pLoadObject->m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, 1, m_MonsterModel);
	m_pLoadObject->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);

	// m_Transform.SetPosition(100 + rand() % 2000, 0, 100 + rand() % 2000);
	m_Transform.SetPosition(1000, 0, 1000);
}

Slime::~Slime()
{
}

void Slime::Update(float fElapsedTime)
{
	m_MonsterMovement->UpdateAI(fElapsedTime, m_pTarget);
	m_MonsterStatus->UpdateAI(fElapsedTime);

	// �̵����� ����Ѵ�. 
	m_MonsterMovement->Update(fElapsedTime);

	// �̵�����ŭ �����δ�. 
	Move(Vector3::ScalarProduct(m_MonsterMovement->m_xmf3Velocity, fElapsedTime, false));

	m_pMyBOBox->SetPosition(
		XMFLOAT3(
			m_Transform.GetPosition().x,
			75.f,
			m_Transform.GetPosition().z)
	);

}
