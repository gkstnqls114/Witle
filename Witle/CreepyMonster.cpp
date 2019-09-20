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
#include "CreepyMonster.h"

#include "SoundManager.h"
 
void CreepyMonster::ReleaseMembers()
{
	Monster::ReleaseMembers();  
}

void CreepyMonster::ReleaseMemberUploadBuffers()
{
	Monster::ReleaseMemberUploadBuffers(); 
}

CreepyMonster::CreepyMonster(const std::string & entityID, const XMFLOAT3& SpawnPoint,
	ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature)
	: Monster(entityID, 500.f, SpawnPoint, pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, CREEPYMONSTER_ATTACK.EndTime - CREEPYMONSTER_ATTACK.StartTime)
{
	m_RecognitionRange = new RecognitionRange(this, 1000.f, 2.f);
	m_RecognitionRange->CreateDebugMesh(pd3dDevice, pd3dCommandList);

	m_MonsterMovement = new MonsterMovement(this);
	m_MonsterMovement->m_fDistance = 100;
	 
	m_MonsterActionMgr = new GeneralMonsterActionMgr(this, 3.f, 7.f);

	m_pTexture = TextureStorage::GetInstance()->GetTexture(CREEPYMONSTER);

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
	m_pMyBOBox = new MyBOBox(this, pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, extents);

	if (rand() % 2)
	{
		static_cast<GeneralMonsterActionMgr*>(m_MonsterActionMgr)->ChangeStateToMove();
	}
	else
	{
		static_cast<GeneralMonsterActionMgr*>(m_MonsterActionMgr)->ChangeStateToIdle();
	}
}

CreepyMonster::~CreepyMonster()
{
}

void CreepyMonster::Init()
{
	Monster::Init(false);

}

void CreepyMonster::Update(float fElapsedTime)
{
	Monster::Update(fElapsedTime);

	// �̵����� ����Ѵ�. 
	m_MonsterMovement->Update(fElapsedTime);

	// ȸ�� ������ ������Ų��.
	m_Transform.SetRotate(m_MonsterMovement->m_fRoll, m_MonsterMovement->m_fPitch, m_MonsterMovement->m_fYaw);

	// �̵�����ŭ �����δ�. 
	Move(Vector3::ScalarProduct(m_MonsterMovement->m_xmf3Velocity, fElapsedTime, false));

}
 
void CreepyMonster::Animate(float fElapsedTime)
{
	Monster::Animate(fElapsedTime);
	
	LoadObject* p = m_pLoadObject->FindFrame("Bip001");
	XMFLOAT3 pos = XMFLOAT3(p->m_xmf4x4World._41, p->m_xmf4x4World._42 + 50, p->m_xmf4x4World._43);

	m_pMyBOBox->SetPosition(pos);
}
