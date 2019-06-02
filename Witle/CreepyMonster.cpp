#include "stdafx.h"

// For Monster ///////////////
#include "MonsterMovement.h"
#include "MonsterStatus.h"
#include "RecognitionRange.h"
#include "PlayerManager.h"
// For Monster ///////////////

#include "ModelStorage.h"
#include "MyDescriptorHeap.h"
#include "Object.h"
#include "MyBOBox.h"
#include "Texture.h"
#include "CreepyMonster.h"
 
void CreepyMonster::ReleaseMembers()
{
	Monster::ReleaseMembers();
	if (m_pHaep)
	{
		m_pHaep->ReleaseObjects();
		delete m_pHaep;
		m_pHaep = nullptr;
	}
	if (m_pTexture)
	{
		m_pTexture->ReleaseObjects();
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

void CreepyMonster::ReleaseMemberUploadBuffers()
{
	Monster::ReleaseMemberUploadBuffers();
	if (m_pTexture)m_pTexture->ReleaseUploadBuffers();
}

CreepyMonster::CreepyMonster(const std::string & entityID, const XMFLOAT3& SpawnPoint,
	ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature)
	: Monster(entityID, SpawnPoint, pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature)
{
	m_pHaep = new MyDescriptorHeap();
	m_pHaep->CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1, 0);
	m_pTexture = new Texture(1, RESOURCE_TEXTURE2D);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/CreepyMonster.dds", 0);
	 
	m_pHaep->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture, ROOTPARAMETER_TEXTURE, false, 0);


	ANIMATION_INFO infos[CREEPYMONSTER_ANIMATIONE];
	infos[0] = CREEPYMONSTER_IDLE;
	infos[1] = CREEPYMONSTER_MOVE;
	infos[2] = CREEPYMONSTER_ATTACK;
	infos[3] = CREEPYMONSTER_DEAD;
	infos[4] = CREEPYMONSTER_HIT;

	m_MonsterModel = LoadObject::LoadGeometryAndAnimationFromFile_forMonster(
		pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/CreepyMonster.bin", NULL,
		CREEPYMONSTER_ANIMATIONE, infos);

	m_pLoadObject = m_MonsterModel->m_pModelRootObject;
	m_pLoadObject->m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, 1, m_MonsterModel);
	m_pLoadObject->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);

	m_Transform.SetPosition(SpawnPoint);


	XMFLOAT3 extents{ 50.f, 50.f, 50.f };
	m_pMyBOBox = new MyBOBox(this, pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 75.F, 0.F }, extents);


	if (rand() % 2)
	{
		m_MonsterMovement->GetMonsterActionMgr()->ChangeStateToMove();
	}
	else
	{
		m_MonsterMovement->GetMonsterActionMgr()->ChangeStateToIdle();
	}
}

CreepyMonster::~CreepyMonster()
{
}

void CreepyMonster::Update(float fElapsedTime)
{
	// �̵����� ����Ѵ�. 
	m_MonsterMovement->Update(fElapsedTime);

	// ȸ�� ������ ������Ų��.
	m_Transform.SetRotate(m_MonsterMovement->m_fRoll, m_MonsterMovement->m_fPitch, m_MonsterMovement->m_fYaw);

	// �̵�����ŭ �����δ�. 
	Move(Vector3::ScalarProduct(m_MonsterMovement->m_xmf3Velocity, fElapsedTime, false));

}

void CreepyMonster::UpdateState(float fElapsedTime)
{
	m_MonsterMovement->UpdateState(fElapsedTime);
}

void CreepyMonster::Animate(float fElapsedTime)
{
	Monster::Animate(fElapsedTime);
	 
	m_pMyBOBox->SetPosition(m_Transform.GetPosition());
}
