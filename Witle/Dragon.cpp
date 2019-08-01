#include "stdafx.h"

// For Monster ///////////////
#include "MonsterMovement.h"
#include "MonsterStatus.h"
#include "RecognitionRange.h"
#include "PlayerManager.h"
#include "BossMonsterActionMgr.h"
// For Monster ///////////////

#include "ModelStorage.h"
#include "MyDescriptorHeap.h"
#include "Object.h"
#include "MyBOBox.h"
#include "Texture.h"
#include "Dragon.h"


void Dragon::ReleaseMembers()
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

void Dragon::ReleaseMemberUploadBuffers()
{
	Monster::ReleaseMemberUploadBuffers();
	if (m_pTexture)m_pTexture->ReleaseUploadBuffers();
}

Dragon::Dragon(const std::string & entityID, const XMFLOAT3& SpawnPoint,
	ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature)
	: Monster(entityID, 100.f, SpawnPoint, pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature)
{
	m_RecognitionRange = new RecognitionRange(this, 2000.f, 2.f);
	m_RecognitionRange->CreateDebugMesh(pd3dDevice, pd3dCommandList);

	m_MonsterMovement = new MonsterMovement(this, 1, 1, true);
	m_MonsterMovement->m_fDistance = 100;

	m_pHaep = new MyDescriptorHeap();
	m_pHaep->CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1, 0);
	m_pTexture = new Texture(1, RESOURCE_TEXTURE2D);

	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/Dragon.dds", 0);

	m_pHaep->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture, ROOTPARAMETER_TEXTURE, false, 0);

	ANIMATION_INFO infos[BOSSMONSTER_ANIMATIONE];
	infos[0] = BOSS_IDLE;
	infos[1] = BOSS_MOVE;
	infos[2] = BOSS_CHASE;
	infos[3] = BOSS_SKILL0;
	infos[4] = BOSS_SKILL1;
	infos[5] = BOSS_SKILL2;
	infos[6] = BOSS_SKILL3;
	infos[7] = BOSS_SKILL4;

	infos[8] = BOSS_SKILL5;
	infos[9] = BOSS_SKILL6;
	infos[10] = BOSS_SKILL7;
	infos[11] = BOSS_SKILL8;
	infos[12] = BOSS_SKILL9;

	infos[13] = BOSS_DEAD;
	infos[14] = BOSS_HIT;

	m_MonsterModel = LoadObject::LoadGeometryAndAnimationFromFile_forMonster(
		pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Dragon.bin", NULL,
		BOSSMONSTER_ANIMATIONE, infos);

	m_pLoadObject = m_MonsterModel->m_pModelRootObject;
	m_pLoadObject->m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, 1, m_MonsterModel);
	m_pLoadObject->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);

	m_Transform.SetPosition(SpawnPoint);

	XMFLOAT3 extents{ 250.f, 150.f, 200.f };
	m_pMyBOBox = new MyBOBox(this, pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 75.F, 0.F }, extents);

	if (rand() % 2)
	{
		static_cast<BossMonsterActionMgr*>(m_MonsterMovement->GetMonsterActionMgr())->ChangeBossStateToMove();
	}
	else
	{
		static_cast<BossMonsterActionMgr*>(m_MonsterMovement->GetMonsterActionMgr())->ChangeBossStateToIdle();
	}
}

Dragon::~Dragon()
{
}

void Dragon::Update(float fElapsedTime)
{
	// 이동량을 계산한다. 
	m_MonsterMovement->Update(fElapsedTime);

	// 회전 각도를 고정시킨다.
	m_Transform.SetRotate(m_MonsterMovement->m_fRoll, m_MonsterMovement->m_fPitch, m_MonsterMovement->m_fYaw);

	// 이동량만큼 움직인다. 
	Move(Vector3::ScalarProduct(m_MonsterMovement->m_xmf3Velocity, fElapsedTime, false));
}

void Dragon::UpdateState(float fElapsedTime)
{
	m_MonsterMovement->UpdateState(fElapsedTime);
}

void Dragon::Animate(float fElapsedTime)
{
	Monster::Animate(fElapsedTime);
	 
	LoadObject* p = m_pLoadObject->FindFrame("Bone001");
	XMFLOAT3 pos = XMFLOAT3(p->m_xmf4x4World._41, p->m_xmf4x4World._42 + 50, p->m_xmf4x4World._43);

	m_pMyBOBox->Rotate(0.f, m_MonsterMovement->m_fPitch, 0.f);
	m_pMyBOBox->SetPosition(pos);
}
