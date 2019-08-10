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


void Dragon::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{ 
#ifdef _DEBUG 
	RenderDebug(pd3dCommandList, isGBuffers);
	m_BOBoxForTailAttack->Render(pd3dCommandList);
#endif // _DEBUG


	if (m_isStone)
	{
		m_pStoneTexture->UpdateShaderVariable(pd3dCommandList, 0); 
	}
	else
	{
		m_pTexture->UpdateShaderVariable(pd3dCommandList, 0);
	}

	m_pLoadObject->Render(pd3dCommandList, isGBuffers);

	RenderHpStatus(pd3dCommandList, isGBuffers);
}

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
	m_pStoneTexture = new Texture(ENUM_SCENE::SCENE_GAME, ROOTPARAMETER_INDEX(ROOTPARAMETER_TEXTURE), false, 1, RESOURCE_TEXTURE2D);
	 
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/Dragon.dds", 0);
	m_pStoneTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/Dragon.dds", 0);

	ANIMATION_INFO infos[BOSSMONSTER_ANIMATIONE];
	infos[0] = BOSS_IDLE;
	infos[1] = BOSS_MOVE;
	infos[2] = BOSS_CHASE;
	infos[3] = BOSS_BRETH;
	infos[4] = BOSS_DOWNSTORK;
	infos[5] = BOSS_TAILATTACK;
	infos[6] = BOSS_RUSH;
	infos[7] = BOSS_BUF;
	infos[8] = BOSS_DEAD;
	infos[9] = BOSS_STONE;

	m_MonsterModel = LoadObject::LoadGeometryAndAnimationFromFile_forMonster(
		pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Dragon.bin", NULL,
		BOSSMONSTER_ANIMATIONE, infos);

	m_pLoadObject = m_MonsterModel->m_pModelRootObject;
	m_pLoadObject->m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, 1, m_MonsterModel);
	m_pLoadObject->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);

	// Bip001_Spine �������� �����ɴϴ�.
	m_BOBoxFrame = m_pLoadObject->FindFrame("Bip001_Spine");

	m_Transform.SetPosition(SpawnPoint);

	XMFLOAT3 extents{ 150.f, 100.f, 230.f };
	m_pMyBOBox = new MyBOBox(this, pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, extents);

	XMFLOAT3 extents_2{ 200.f, 200.f, 400.f };
	m_BOBoxForTailAttack = new MyBOBox(this, pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, extents_2);

	static_cast<BossMonsterActionMgr*>(m_MonsterMovement->GetMonsterActionMgr())->ChangeBossStateToStone();
}

Dragon::~Dragon()
{
}

void Dragon::Rotate(float x, float y, float z)
{
	Monster::Rotate(x, y, z);
	m_BOBoxForTailAttack->Rotate(m_MonsterMovement->m_fRoll, m_MonsterMovement->m_fYaw, m_MonsterMovement->m_fPitch);
}

void Dragon::Update(float fElapsedTime)
{ 
	if (m_isStone) return;

	Monster::Update(fElapsedTime);

	// �̵����� ����Ѵ�. 
	m_MonsterMovement->Update(fElapsedTime);

	// ȸ�� ������ ������Ų��.
	m_Transform.SetRotate(m_MonsterMovement->m_fRoll, m_MonsterMovement->m_fPitch, m_MonsterMovement->m_fYaw);

	// �̵�����ŭ �����δ�. 
	Move(Vector3::ScalarProduct(m_MonsterMovement->m_xmf3Velocity, fElapsedTime, false));
}

void Dragon::UpdateState(float fElapsedTime)
{
	if (m_isStone) return;

	m_MonsterMovement->UpdateState(fElapsedTime);

	m_MonsterMovement->UpdateVelocity(fElapsedTime); // State ���¿� ���� Velocity�� ����(Set)�Ѵ�.
}

void Dragon::Animate(float fElapsedTime)
{
	// ���� �ִϸ��̼ǿ� ���� ����� ������Ʈ�մϴ�.
	Monster::Animate(fElapsedTime);

	// m_BOBoxFrame�� Bip001_Spine ���� ��������� �����ɴϴ�.
	// �ش� �����ӿ� ���߾� �ٿ�� �ڽ� ��ġ�� ȸ���� �缳�� �մϴ�.
	XMFLOAT3 pos = XMFLOAT3(m_BOBoxFrame->m_xmf4x4World._41, m_BOBoxFrame->m_xmf4x4World._42, m_BOBoxFrame->m_xmf4x4World._43);
	XMFLOAT3 offset = Vector3::ScalarProduct(m_Transform.GetLook(), 100.f, false);
	XMFLOAT3 result_pos = Vector3::Add(pos, offset);

	m_pMyBOBox->Rotate(0.f, m_MonsterMovement->m_fPitch, 0.f);
	m_pMyBOBox->SetPosition(result_pos);
	 
	m_BOBoxForTailAttack->Rotate(0.f, m_MonsterMovement->m_fPitch, 0.f);
	m_BOBoxForTailAttack->SetPosition(result_pos);
}

MyBOBox * Dragon::GetBOBox() const
{
	if (static_cast<BossMonsterActionMgr*>(m_MonsterMovement->GetMonsterActionMgr())->Is_BossSkillTailAttack())
	{
		return m_BOBoxForTailAttack;
	}
	return m_pMyBOBox;
}

void Dragon::IsStone()
{
	m_isStone = true;
	static_cast<BossMonsterActionMgr*>(m_MonsterMovement->GetMonsterActionMgr())->ChangeBossStateToStone();
}

void Dragon::IsNotStone()
{
	m_isStone = false;
	static_cast<BossMonsterActionMgr*>(m_MonsterMovement->GetMonsterActionMgr())->ChangeBossStateToIdle();
}
