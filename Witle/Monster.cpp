#include "stdafx.h"

// Game Base /////////////////////
#include "GameInput.h"
#include "GameTimer.h"
#include "GameScreen.h"
// Game Base /////////////////////

// For Monster /////////////////////
#include "MonsterMovement.h"
#include "RecognitionRange.h"
#include "MonsterStatus.h"
#include "UI3DImage.h"
// GameBase /////////////////////

// DebugMesh /////////////////////
#include "MyBOBox.h" 
#include "LineSphere.h"
// DebugMesh /////////////////////

#include "GeneralMonsterActionMgr.h"
#include "BossMonsterActionMgr.h"
#include "CameraObject.h"
#include "MainCameraMgr.h"
#include "Texture.h"
#include "MyDescriptorHeap.h"
#include "Sniping.h"
#include "EffectRect.h"
#include "MyRectangle.h"
#include "Object.h"
#include "ShaderManager.h" 
#include "Transform.h"
#include "FollowCam.h"
#include "Terrain.h"

#include "Monster.h"

#include "SoundManager.h"

static float MonsterOffestX = 0.f;
static float MonsterOffestY = 57.f;
static float MonsterOffestZ = 50.f;
 
XMFLOAT3 Monster::CalculateAlreadyVelocity(float fTimeElapsed)
{
	XMFLOAT3 AlreadyVelocity = Vector3::Add(m_MonsterMovement->GetVelocity(), m_MonsterMovement->m_xmf3Gravity);
	float fLength = sqrtf(AlreadyVelocity.x * AlreadyVelocity.x + AlreadyVelocity.z * AlreadyVelocity.z);
	float fMaxVelocityXZ = m_MonsterMovement->m_fMaxVelocityXZ;
	if (fLength > m_MonsterMovement->m_fMaxVelocityXZ)
	{
		AlreadyVelocity.x *= (fMaxVelocityXZ / fLength);
		AlreadyVelocity.z *= (fMaxVelocityXZ / fLength);
	}
	float fMaxVelocityY = m_MonsterMovement->m_fMaxVelocityY;
	fLength = sqrtf(AlreadyVelocity.y * AlreadyVelocity.y);
	if (fLength > m_MonsterMovement->m_fMaxVelocityY) AlreadyVelocity.y *= (fMaxVelocityY / fLength);

	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(AlreadyVelocity, fTimeElapsed, false);

	return xmf3Velocity;
}
BoundingOrientedBox Monster::CalculateAlreadyBoundingBox(float fTimeElapsed)
{
	XMFLOAT3 AlreadyPosition = Vector3::Add(m_Transform.GetPosition(), m_MonsterMovement->AlreadyUpdate(fTimeElapsed));
	BoundingOrientedBox AlreadyBBox = m_pMyBOBox->GetBOBox();
	AlreadyBBox.Center = AlreadyPosition;
	return AlreadyBBox;
}
XMFLOAT3 Monster::CalculateAlreadyPosition(float fTimeElapsed)
{
	XMFLOAT3 AlreadyPosition = Vector3::Add(m_Transform.GetPosition(), m_MonsterMovement->AlreadyUpdate(fTimeElapsed));
	return AlreadyPosition;
}


Monster::Monster(const std::string & entityID, float spawnRange, const XMFLOAT3& SpawnPoint, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, float attackanimeTime, float HpBarY)
	: m_SpawnRange(spawnRange), m_SpawnPoint(SpawnPoint), GameObject(entityID), m_FinishAttackUnlockTime(attackanimeTime), m_HpBarY(HpBarY)
{
	m_MonsterHPStatus = new MonsterStatus(this, pd3dDevice, pd3dCommandList);
	
	m_MonsterHPUI = new UI3DImage(this, pd3dDevice, pd3dCommandList, POINT{0, 0},
		100.F,
		30.f,
		IMAGE_RED
	);
	  
#ifdef SHOW_DEBUGMESH 
	m_pDebugObject = new EmptyGameObject("SpawnPosition");
	m_pDebugObject->GetTransform().SetPosition(SpawnPoint);
	m_pDebugObject->GetTransform().Update(0.f); // position update위해...
	m_pDebugSpawnMesh = new LineSphere(m_pDebugObject, pd3dDevice, pd3dCommandList, XMFLOAT4(0, 0, 1, 0), m_SpawnRange, m_SpawnRange);
#endif // SHOW_DEBUGMESH  
}

Monster::~Monster()
{
	 
}

void Monster::Init(bool isBoss)
{
	m_MonsterHPStatus->SetGuage(100.f);

	m_pLoadObject->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);
	m_Transform.SetPosition(m_SpawnPoint);

	if (isBoss)
	{
		static_cast<BossMonsterActionMgr*>(m_MonsterActionMgr)->ChangeBossStateToStone();
	}
	else
	{
		if (rand() % 2)
		{
			static_cast<GeneralMonsterActionMgr*>(m_MonsterActionMgr)->ChangeStateToMove();
		}
		else
		{
		static_cast<GeneralMonsterActionMgr*>(m_MonsterActionMgr)->ChangeStateToIdle();
		}
	}

}

void Monster::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	RenderDebug(pd3dCommandList, isGBuffers);

	m_pTexture->UpdateShaderVariable(pd3dCommandList, 0);
	m_pLoadObject->Render(pd3dCommandList, isGBuffers);

	RenderHpStatus(pd3dCommandList, isGBuffers);
}

void Monster::RenderForShadow(ID3D12GraphicsCommandList * pd3dCommandList)
{ 
	m_pLoadObject->RenderForShadow(pd3dCommandList);
}

void Monster::Update(float fElapsedTime)
{
	SoundManager::GetInstance()->UpdatesetAttributesPos(this);

	if (m_isFinishAttack)
	{
		m_TotalTime += fElapsedTime;
		if (m_TotalTime >= m_FinishAttackUnlockTime)
		{
			m_TotalTime = 0.f;
			m_isFinishAttack = false;
		}
	}
}

void Monster::UpdateState(float fElapsedTime)
{
	m_MonsterActionMgr->UpdateState(fElapsedTime);
	m_MonsterActionMgr->UpdateVelocity(fElapsedTime, m_MonsterMovement); // State 상태에 따라 Velocity를 설정한다.
}

void Monster::ReleaseMembers()
{
#ifdef SHOW_DEBUGMESH
	if (m_pDebugObject)
	{
		m_pDebugObject->ReleaseObjects();
		delete m_pDebugObject;
		m_pDebugObject = nullptr;
	}
	if (m_pDebugSpawnMesh)
	{
		m_pDebugSpawnMesh->ReleaseObjects();
		delete m_pDebugSpawnMesh;
		m_pDebugSpawnMesh = nullptr;
	}
#endif // SHOW_DEBUGMESH 

	if (m_MonsterHPUI)
	{
		m_MonsterHPUI->ReleaseObjects();
		delete m_MonsterHPUI;
		m_MonsterHPUI = nullptr;
	}
	if (m_MonsterActionMgr)
	{ 
		m_MonsterActionMgr->ReleaseObjects();
		delete m_MonsterActionMgr;
		m_MonsterActionMgr = nullptr;
	}
	if (m_RecognitionRange)
	{
		m_RecognitionRange->ReleaseObjects();
		delete m_RecognitionRange;
		m_RecognitionRange = nullptr;
	}
	if (m_pLoadObject)
	{
		m_pLoadObject->ReleaseObjects();
		m_pLoadObject = nullptr;
	}

	if (m_MonsterModel)
	{
		m_MonsterModel->ReleaseObjects();
		delete m_MonsterModel;
		m_MonsterModel = nullptr;
	}
	if (m_pMyBOBox)
	{
		m_pMyBOBox->ReleaseObjects();
		delete m_pMyBOBox;
		m_pMyBOBox = nullptr;
	}
	if (m_MonsterHPStatus)
	{
		m_MonsterHPStatus->ReleaseObjects();
		delete m_MonsterHPStatus;
		m_MonsterHPStatus = nullptr;
	}
	if (m_MonsterMovement)
	{
		m_MonsterMovement->ReleaseObjects();
		delete m_MonsterMovement;
		m_MonsterMovement = nullptr;
	}
}

void Monster::ReleaseMemberUploadBuffers()
{
#ifdef SHOW_DEBUGMESH
	if (m_pDebugSpawnMesh) m_pDebugSpawnMesh->ReleaseUploadBuffers();
#endif // SHOW_DEBUGMESH

	if (m_MonsterHPUI) m_MonsterHPUI->ReleaseUploadBuffers();
	if (m_RecognitionRange) m_RecognitionRange->ReleaseUploadBuffers();
	if (m_pLoadObject) m_pLoadObject->ReleaseUploadBuffers();
	if (m_MonsterModel)m_MonsterModel->ReleaseUploadBuffers();
	if (m_pMyBOBox)m_pMyBOBox->ReleaseUploadBuffers();
}


void Monster::SubstractHP(int sub)
{
	static_cast<GeneralMonsterActionMgr*>(m_MonsterActionMgr)->ChangeStateToHit();
 
	m_MonsterHPStatus->SubstractHP(sub); 
}

void Monster::Animate(float fElapsedTime)
{ 
	// 반드시 트랜스폼 업데이트..! 
	m_Transform.Update(fElapsedTime);

	m_pLoadObject->m_xmf4x4ToParent = m_Transform.GetWorldMatrix();

	m_pLoadObject->Animate(fElapsedTime);
}

void Monster::RenderDebug(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
#ifdef SHOW_DEBUGMESH 
	m_pMyBOBox->Render(pd3dCommandList);
	// m_pDebugSpawnMesh->Render(pd3dCommandList, isGBuffers);
	m_RecognitionRange->Render(pd3dCommandList);
#endif // SHOW_DEBUGMESH 
}

void Monster::RenderHpStatus(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_UIWORLD, isGBuffers);

	// set look at.... 빌보드 처리...
	XMFLOAT4X4 uiWorld = m_Transform.GetWorldMatrix();
	uiWorld._42 += m_HpBarY;

	XMFLOAT3 xmf3Position(uiWorld._41, uiWorld._42, uiWorld._43);

	XMFLOAT4X4 mtxLookAt = Matrix4x4::LookAtLH(xmf3Position, MainCameraMgr::GetMainCamera()->GetTransform().GetPosition(), XMFLOAT3(0, 1, 0));
	uiWorld._11 = mtxLookAt._11; uiWorld._12 = mtxLookAt._21; uiWorld._13 = mtxLookAt._31;
	uiWorld._21 = mtxLookAt._12; uiWorld._22 = mtxLookAt._22; uiWorld._23 = mtxLookAt._32;
	uiWorld._31 = mtxLookAt._13; uiWorld._32 = mtxLookAt._23; uiWorld._33 = mtxLookAt._33;
	// set look at....

	float percentage = float(m_MonsterHPStatus->GetGuage()) / float(m_MonsterHPStatus->m_MAXGuage) * 100.f;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &percentage, 0);

	m_MonsterHPUI->Render(pd3dCommandList, uiWorld);
}

void Monster::Move(const XMFLOAT3 & xmf3Shift)
{
	m_Transform.Move(xmf3Shift);
	// SoundManager::GetInstance()->Play(ENUM_SOUND::SPACE_MOVE);
}

void Monster::Rotate(float x, float y, float z)
{
	m_Transform.Rotate(x, y, z);
	m_pMyBOBox->Rotate(m_MonsterMovement->m_fRoll, m_MonsterMovement->m_fYaw, m_MonsterMovement->m_fPitch);
}

Movement* const Monster::GetpMovement() const
{
	return static_cast<Movement * const>(m_MonsterMovement);
}

XMFLOAT3 Monster::GetVelocity() const
{
	return m_MonsterMovement->GetVelocity();
}

void Monster::SetVelocity(const XMFLOAT3 & velocity)
{
	m_MonsterMovement->SetVelocity(velocity);
}

void Monster::SetAnimationID(ENUM_ANIMATIONID state)
{ 
	m_MonsterActionMgr->ProcessActions();
	m_pLoadObject->SetTrackAnimationSet(0, state);
}
