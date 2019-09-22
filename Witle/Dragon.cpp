#include "stdafx.h"

// For Monster ///////////////
#include "MonsterMovement.h"
#include "MonsterStatus.h"
#include "RecognitionRange.h"
#include "PlayerManager.h"
#include "BossMonsterActionMgr.h"
// For Monster ///////////////

#include "SceneMgr.h"
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


	if (!m_isStone) 
	{
		RenderHpStatus(pd3dCommandList, isGBuffers);
	}
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

float Dragon::GetFinishAttackUnlockTime()
{
	BossMonsterActionMgr* actionmgr = static_cast<BossMonsterActionMgr*>(m_MonsterActionMgr);

	if (actionmgr->Is_BossSkillBreath())
	{
		return BOSS_BREATH.EndTime - BOSS_BREATH.StartTime;
	}
	else if (actionmgr->Is_BossSkillDownStroke())
	{
		return BOSS_DOWNSTORK.EndTime - BOSS_DOWNSTORK.StartTime;
	}
	else if (actionmgr->Is_BossSkillTailAttack())
	{
		return BOSS_TAILATTACK.EndTime - BOSS_TAILATTACK.StartTime;
	}
	else if (actionmgr->Is_BossSkillRush())
	{ 
		return BOSS_RUSH.EndTime - BOSS_RUSH.StartTime;
	}

	return 0.f;
}

Dragon::Dragon(const std::string & entityID, const XMFLOAT3& SpawnPoint,
	ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature)
	: Monster(entityID, 100.f, SpawnPoint, pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, 0.f ,400.f)
{ 
	m_RecognitionRange = new RecognitionRange(this, 2000.f, 2.f);
	m_RecognitionRange->CreateDebugMesh(pd3dDevice, pd3dCommandList);

	m_MonsterMovement = new MonsterMovement(this);
	m_MonsterMovement->m_fDistance = 150;

	m_MonsterActionMgr = new BossMonsterActionMgr(this, 1, 1);
	
	m_pTexture = new Texture(ENUM_SCENE::SCENE_GAME, ROOTPARAMETER_INDEX(ROOTPARAMETER_TEXTURE), false, 1, RESOURCE_TEXTURE2D);
	m_pStoneTexture = new Texture(ENUM_SCENE::SCENE_GAME, ROOTPARAMETER_INDEX(ROOTPARAMETER_TEXTURE), false, 1, RESOURCE_TEXTURE2D);
	 
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/Dragon.dds", 0);
	m_pStoneTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/Boss_rock.dds", 0);

	ANIMATION_INFO infos[BOSSMONSTER_ANIMATIONE];
	infos[0] = BOSS_IDLE;
	infos[1] = BOSS_MOVE;
	infos[2] = BOSS_CHASE;
	infos[3] = BOSS_BREATH;
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
	
	// Bip001_Spine �������� �����ɴϴ�.
	m_BOBoxFrame = m_pLoadObject->FindFrame("Bip001_Spine");
	m_HeadFrame = m_pLoadObject->FindFrame("Bip001_Head");
	 
	XMFLOAT3 extents{ 150.f, 100.f, 230.f };
	m_pMyBOBox = new MyBOBox(this, pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, extents);

	XMFLOAT3 extents_2{ 200.f, 200.f, 500.f };
	m_BOBoxForTailAttack = new MyBOBox(this, pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, extents_2);
	 
	Init();
}

Dragon::~Dragon()
{
}

void Dragon::Init()
{    
	Monster::Init(true);
	m_MonsterActionMgr->Init();

	IsStone(); 
}

void Dragon::SubstractHP(int sub)
{
	if (m_isStone) return;

	// hit �ִϸ��̼� ����
	m_MonsterHPStatus->SubstractHP(sub); 
}

void Dragon::Rotate(float x, float y, float z)
{
	Monster::Rotate(x, y, z);
	m_BOBoxForTailAttack->Rotate(m_MonsterMovement->m_fRoll, m_MonsterMovement->m_fYaw, m_MonsterMovement->m_fPitch);
}

void Dragon::Update(float fElapsedTime)
{
	bool isDead = static_cast<BossMonsterActionMgr*>(m_MonsterActionMgr)->Is_BossDead();
	if (isDead && m_pLoadObject->IsTrackAnimationSetFinish(0, BOSS_DEAD.ID))
	{ 
		static_cast<BossMonsterActionMgr*>(m_MonsterActionMgr)->ChangeBossStateToStone();
		SceneMgr::GetInstacne()->ChangeSceneToWin();
		return;
	}
	if (isDead) return;

	if (m_isStone) return;
	 
	if (m_isFinishAttack)
	{
		m_TotalTime += fElapsedTime;
		if (m_TotalTime >= GetFinishAttackUnlockTime())
		{
			m_TotalTime = 0.f;
			m_isFinishAttack = false;
		}
	}

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

	m_MonsterActionMgr->UpdateState(fElapsedTime);
	m_MonsterActionMgr->UpdateVelocity(fElapsedTime, m_MonsterMovement); // State ���¿� ���� Velocity�� �����Ѵ�.
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
	if (static_cast<BossMonsterActionMgr*>(m_MonsterActionMgr)->Is_BossSkillTailAttack() 
		|| static_cast<BossMonsterActionMgr*>(m_MonsterActionMgr)->Is_BossSkillDownStroke()
		|| static_cast<BossMonsterActionMgr*>(m_MonsterActionMgr)->Is_BossSkillRush()
		)
	{
		return m_BOBoxForTailAttack;
	}
	return m_pMyBOBox;
}

void Dragon::IsStone()
{
	m_isStone = true;
	static_cast<BossMonsterActionMgr*>(m_MonsterActionMgr)->ChangeBossStateToStone();
}

void Dragon::IsNotStone()
{
	m_isStone = false;
	static_cast<BossMonsterActionMgr*>(m_MonsterActionMgr)->ChangeBossStateToIdle();
}
