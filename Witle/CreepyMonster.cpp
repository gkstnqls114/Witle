#include "stdafx.h"

// For Monster ///////////////
#include "MonsterMovement.h"
#include "MonsterStatus.h"
#include "RecognitionRange.h"
#include "PlayerManager.h"
#include "GeneralMonsterActionMgr.h"
// For Monster ///////////////

#include "ModelStorage.h"
#include "MyDescriptorHeap.h"
#include "Object.h"
#include "MyBOBox.h"
#include "Texture.h"
#include "CreepyMonster.h"

#include "SoundManager.h"
 
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
	: Monster(entityID, 500.f, SpawnPoint, pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature)
{
	m_RecognitionRange = new RecognitionRange(this, 1000.f, 2.f);
	m_RecognitionRange->CreateDebugMesh(pd3dDevice, pd3dCommandList);

	m_MonsterMovement = new MonsterMovement(this, 1, 1);
	m_MonsterMovement->m_fDistance = 100;

	m_pHaep = new MyDescriptorHeap();
	m_pHaep->CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1, 0, ENUM_SCENE::SCENE_GAME);
	m_pTexture = new Texture(1, RESOURCE_TEXTURE2D);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/CreepyMonster.dds", 0);
	 
	m_pHaep->CreateShaderResourceViews(pd3dDevice,  m_pTexture, ROOTPARAMETER_TEXTURE, false, 0);


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
		static_cast<GeneralMonsterActionMgr*>(m_MonsterMovement->GetMonsterActionMgr())->ChangeStateToMove();
	}
	else
	{
		static_cast<GeneralMonsterActionMgr*>(m_MonsterMovement->GetMonsterActionMgr())->ChangeStateToIdle();
	}
}

CreepyMonster::~CreepyMonster()
{
}

void CreepyMonster::Update(float fElapsedTime)
{
	// 이동량을 계산한다. 
	m_MonsterMovement->Update(fElapsedTime);

	// 회전 각도를 고정시킨다.
	m_Transform.SetRotate(m_MonsterMovement->m_fRoll, m_MonsterMovement->m_fPitch, m_MonsterMovement->m_fYaw);

	// 이동량만큼 움직인다. 
	Move(Vector3::ScalarProduct(m_MonsterMovement->m_xmf3Velocity, fElapsedTime, false));

}

void CreepyMonster::UpdateState(float fElapsedTime)
{
	m_MonsterMovement->UpdateState(fElapsedTime);
}

void CreepyMonster::Animate(float fElapsedTime)
{
	Monster::Animate(fElapsedTime);
	
	LoadObject* p = m_pLoadObject->FindFrame("Bip001");
	XMFLOAT3 pos = XMFLOAT3(p->m_xmf4x4World._41, p->m_xmf4x4World._42 + 50, p->m_xmf4x4World._43);

	m_pMyBOBox->SetPosition(pos);
}
