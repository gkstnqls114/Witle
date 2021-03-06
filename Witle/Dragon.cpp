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
	RenderDebug(pd3dCommandList, isGBuffers);
	m_BOBoxForTailAttack->Render(pd3dCommandList); 
	 
	if (m_isStone)
	{
		m_pStoneTexture->UpdateShaderVariable(pd3dCommandList, 0); 
	}
	else
	{
		m_pTexture->UpdateShaderVariable(pd3dCommandList, 0);
	}

	if(m_pLoadObject) m_pLoadObject->Render(pd3dCommandList, isGBuffers);
	 
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
	if (m_pStoneTexture)
	{ 
		m_pStoneTexture->ReleaseObjects();
		delete m_pStoneTexture;
		m_pStoneTexture = nullptr;
	}

	if (m_pMyBOBox)
	{
		m_pMyBOBox->ReleaseObjects();
		delete m_pMyBOBox;
		m_pMyBOBox = nullptr;
	}
	if (m_BOBoxForTailAttack)
	{
		m_BOBoxForTailAttack->ReleaseObjects();
		delete m_BOBoxForTailAttack;
		m_BOBoxForTailAttack = nullptr;
	}
	
}

void Dragon::ReleaseMemberUploadBuffers()
{
	Monster::ReleaseMemberUploadBuffers();
	if (m_pTexture)m_pTexture->ReleaseUploadBuffers();
	if (m_pStoneTexture)m_pStoneTexture->ReleaseUploadBuffers();
	if (m_pMyBOBox) m_pMyBOBox->ReleaseUploadBuffers();
	if (m_BOBoxForTailAttack) m_BOBoxForTailAttack->ReleaseUploadBuffers();
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
	
	// Bip001_Spine 프레임을 가져옵니다.
	m_pBOBoxFrame = m_pLoadObject->FindFrame("Bip001_Spine");
	m_pHeadFrame = m_pLoadObject->FindFrame("Bip001_Head");
	 
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
	m_Transform.SetRotate(0.f, 0.f, 0.f);
	m_MonsterActionMgr->Init();

	IsStone(); 
}

void Dragon::SubstractHP(int sub)
{
	if (m_isStone) return;

	// hit 애니메이션 없음
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
		SceneMgr::GetInstance()->ChangeSceneToWin();
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

	// 이동량을 계산한다. 
	m_MonsterMovement->Update(fElapsedTime);

	// 회전 각도를 고정시킨다.
	m_Transform.SetRotate(m_MonsterMovement->m_fRoll, m_MonsterMovement->m_fPitch, m_MonsterMovement->m_fYaw);

	// 이동량만큼 움직인다. 
	Move(Vector3::ScalarProduct(m_MonsterMovement->GetVelocity(), fElapsedTime, false));
}

void Dragon::UpdateState(float fElapsedTime)
{
	if (m_isStone) return;

	m_MonsterActionMgr->UpdateState(fElapsedTime);
	m_MonsterActionMgr->UpdateVelocity(fElapsedTime, m_MonsterMovement); // State 상태에 따라 Velocity를 설정한다.
}

void Dragon::Animate(float fElapsedTime)
{
	// 몬스터 애니메이션에 대한 행렬을 업데이트합니다.
	Monster::Animate(fElapsedTime);

	// m_pBOBoxFrame은 Bip001_Spine 상의 월드행렬을 가져옵니다.
	// 해당 프레임에 맞추어 바운딩 박스 위치와 회전을 재설정 합니다.
	XMFLOAT3 pos = XMFLOAT3(m_pBOBoxFrame->m_xmf4x4World._41, m_pBOBoxFrame->m_xmf4x4World._42, m_pBOBoxFrame->m_xmf4x4World._43);
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
	if (m_isStone) return;
	m_isStone = true;
	static_cast<BossMonsterActionMgr*>(m_MonsterActionMgr)->ChangeBossStateToStone();
}

void Dragon::IsNotStone()
{
	if (!m_isStone) return;
	m_isStone = false;
	static_cast<BossMonsterActionMgr*>(m_MonsterActionMgr)->ChangeBossStateToIdle();
}
