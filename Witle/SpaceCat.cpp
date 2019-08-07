#include "stdafx.h"

// For Monster ///////////////
#include "MonsterMovement.h"
#include "MonsterStatus.h"
#include "RecognitionRange.h"
#include "PlayerManager.h"
#include "GeneralMonsterActionMgr.h"
// For Monster ///////////////

#include "TextureStorage.h"
#include "ModelStorage.h"
#include "MyDescriptorHeap.h"
#include "Object.h"
#include "MyBOBox.h"
#include "Texture.h"
#include "SpaceCat.h"

void SpaceCat::ReleaseMembers()
{
	Monster::ReleaseMembers();

	if (m_pTexture)
	{
		m_pTexture->ReleaseObjects();
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

void SpaceCat::ReleaseMemberUploadBuffers()
{
	Monster::ReleaseMemberUploadBuffers();
	if (m_pTexture)m_pTexture->ReleaseUploadBuffers();
}

SpaceCat::SpaceCat(const std::string & entityID, const XMFLOAT3& SpawnPoint,
	ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature)
	: Monster(entityID, 100.f, SpawnPoint, pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature)
{
	m_RecognitionRange = new RecognitionRange(this, 500.f, 2.f);
	m_RecognitionRange->CreateDebugMesh(pd3dDevice, pd3dCommandList);

	m_MonsterMovement = new MonsterMovement(this, 1, 1);
	m_MonsterMovement->m_fDistance = 100;
	
	int val = rand() % 3;
	if (val == 0)
	{
		m_pTexture = TextureStorage::GetInstance()->GetTexture(SPACECAT_GREEN);
	}
	else if (val == 1)
	{
		m_pTexture = TextureStorage::GetInstance()->GetTexture(SPACECAT_BLUE);
	}
	else
	{
		m_pTexture = TextureStorage::GetInstance()->GetTexture(SPACECAT_PINK);
	}
	 
	ANIMATION_INFO infos[SPACECAT_ANIMATIONE];
	infos[0] = SPACECAT_IDLE;
	infos[1] = SPACECAT_MOVE;
	infos[2] = SPACECAT_ATTACK;
	infos[3] = SPACECAT_DEAD;
	infos[4] = SPACECAT_HIT;

	m_MonsterModel = LoadObject::LoadGeometryAndAnimationFromFile_forMonster(
		pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/SpaceCat.bin", NULL,
		SPACECAT_ANIMATIONE, infos);

	// m_MonsterModel = ModelStorage::GetInstance()->GetModelInfo(SPACECAT);

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

SpaceCat::~SpaceCat()
{
}

void SpaceCat::Update(float fElapsedTime)
{
	// �̵����� ����Ѵ�. 
	m_MonsterMovement->Update(fElapsedTime);

	// ȸ�� ������ ������Ų��.
	m_Transform.SetRotate(m_MonsterMovement->m_fRoll, m_MonsterMovement->m_fPitch, m_MonsterMovement->m_fYaw);

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

	LoadObject* p = m_pLoadObject->FindFrame("Bone001");
	XMFLOAT3 pos = XMFLOAT3(p->m_xmf4x4World._41, p->m_xmf4x4World._42 + 50, p->m_xmf4x4World._43);

	m_pMyBOBox->SetPosition(pos);
}
