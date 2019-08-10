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
	: Monster(entityID, 100.f, SpawnPoint, pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, 0.f ,400.f)
{

	m_RecognitionRange = new RecognitionRange(this, 2000.f, 2.f);
	m_RecognitionRange->CreateDebugMesh(pd3dDevice, pd3dCommandList);

	m_MonsterMovement = new MonsterMovement(this, 1, 1, true);
	m_MonsterMovement->m_fDistance = 100;
	 
	m_pTexture = new Texture(ENUM_SCENE::SCENE_GAME, ROOTPARAMETER_INDEX(ROOTPARAMETER_TEXTURE), false, 1, RESOURCE_TEXTURE2D);

	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/Dragon.dds", 0);
	 
	ANIMATION_INFO infos[BOSSMONSTER_ANIMATIONE];
	infos[0] = BOSS_IDLE;
	infos[1] = BOSS_MOVE;
	infos[2] = BOSS_CHASE;
	infos[3] = BOSS_BRETH;
	infos[4] = BOSS_DOWNSTORK;
	infos[5] = BOSS_TAILATTACK;
	infos[6] = BOSS_RUSH;
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

	// Bip001_Spine 프레임을 가져옵니다.
	m_BOBoxFrame = m_pLoadObject->FindFrame("Bip001_Spine");

	m_Transform.SetPosition(SpawnPoint);

	XMFLOAT3 extents{ 150.f, 100.f, 230.f };
	m_pMyBOBox = new MyBOBox(this, pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, extents);

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
	Monster::Update(fElapsedTime);

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

	m_MonsterMovement->UpdateVelocity(fElapsedTime); // State 상태에 따라 Velocity를 갱신(Set)한다.
}

void Dragon::Animate(float fElapsedTime)
{
	// 몬스터 애니메이션에 대한 행렬을 업데이트합니다.
	Monster::Animate(fElapsedTime);

	// m_BOBoxFrame은 Bip001_Spine 상의 월드행렬을 가져옵니다.
	// 해당 프레임에 맞추어 바운딩 박스 위치와 회전을 재설정 합니다.
	XMFLOAT3 pos = XMFLOAT3(m_BOBoxFrame->m_xmf4x4World._41, m_BOBoxFrame->m_xmf4x4World._42, m_BOBoxFrame->m_xmf4x4World._43);
	XMFLOAT3 offset = Vector3::ScalarProduct(m_Transform.GetLook(), 100.f, false);
	XMFLOAT3 result_pos = Vector3::Add(pos, offset);

	m_pMyBOBox->Rotate(0.f, m_MonsterMovement->m_fPitch, 0.f);
	m_pMyBOBox->SetPosition(result_pos);
}
