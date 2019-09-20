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
#include "Mushroom.h"

void Mushroom::ReleaseMembers()
{
	Monster::ReleaseMembers(); 
}

void Mushroom::ReleaseMemberUploadBuffers()
{
	Monster::ReleaseMemberUploadBuffers(); 
}

Mushroom::Mushroom(const std::string & entityID, const XMFLOAT3& SpawnPoint,
	ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature)
	: Monster(entityID, 100.f, SpawnPoint, pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, ANI_MUSHROOM_ATTACK.EndTime - ANI_MUSHROOM_ATTACK.StartTime)
{
	m_RecognitionRange = new RecognitionRange(this, 500.f, 2.f);
	m_RecognitionRange->CreateDebugMesh(pd3dDevice, pd3dCommandList);

	m_MonsterMovement = new MonsterMovement(this);
	m_MonsterMovement->m_fDistance = 100;

	m_MonsterActionMgr = new GeneralMonsterActionMgr(this, 1.f, 5.f);

	m_pTexture = TextureStorage::GetInstance()->GetTexture(MUSHROOM);

	ANIMATION_INFO infos[MUSHROOM_ANIMATIONE];
	infos[0] = ANI_MUSHROOM_IDLE;
	infos[1] = ANI_MUSHROOM_MOVE;
	infos[2] = ANI_MUSHROOM_ATTACK;
	infos[3] = ANI_MUSHROOM_DEAD;
	infos[4] = ANI_MUSHROOM_HIT;

	m_MonsterModel = LoadObject::LoadGeometryAndAnimationFromFile_forMonster(
		pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Mashroom.bin", NULL,
		MUSHROOM_ANIMATIONE, infos);

	// m_MonsterModel = ModelStorage::GetInstance()->GetModelInfo(Mushroom);

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

Mushroom::~Mushroom()
{
}

void Mushroom::Init()
{
	Monster::Init(false);

}

void Mushroom::Update(float fElapsedTime)
{
	Monster::Update(fElapsedTime);

	// 이동량을 계산한다. 
	m_MonsterMovement->Update(fElapsedTime);

	// 회전 각도를 고정시킨다.
	m_Transform.SetRotate(m_MonsterMovement->m_fRoll, m_MonsterMovement->m_fPitch, m_MonsterMovement->m_fYaw);

	// 이동량만큼 움직인다. 
	Move(Vector3::ScalarProduct(m_MonsterMovement->m_xmf3Velocity, fElapsedTime, false));

}
 
void Mushroom::Animate(float fElapsedTime)
{
	Monster::Animate(fElapsedTime);

	LoadObject* p = m_pLoadObject->FindFrame("Bone001");
	XMFLOAT3 pos = XMFLOAT3(p->m_xmf4x4World._41, p->m_xmf4x4World._42 + 50, p->m_xmf4x4World._43);

	m_pMyBOBox->SetPosition(pos);
}
