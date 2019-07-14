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

static float MonsterOffestX = 0.f;
static float MonsterOffestY = 57.f;
static float MonsterOffestZ = 50.f;

bool Monster::RENDER_DEBUG{ true };

XMFLOAT3 Monster::CalculateAlreadyVelocity(float fTimeElapsed)
{
	XMFLOAT3 AlreadyVelocity = Vector3::Add(m_MonsterMovement->m_xmf3Velocity, m_MonsterMovement->m_xmf3Gravity);
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
 

Monster::Monster(const std::string & entityID, float spawnRange, const XMFLOAT3& SpawnPoint, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature)
	: m_SpawnRange(spawnRange), m_SpawnPoint(SpawnPoint), GameObject(entityID)
{
	m_MonsterStatus = new MonsterStatus(this, pd3dDevice, pd3dCommandList);
	
	m_MonsterHP = new UI3DImage(this, pd3dDevice, pd3dCommandList, POINT{0, 0},
		100.F,
		30.f,
		L"Image/Red.dds"
	);

	// 디버그용
	m_pDebugObject = new EmptyGameObject("SpawnPosition");
	m_pDebugObject->GetTransform().SetPosition(SpawnPoint);
	m_pDebugObject->GetTransform().Update(0.f); // position update위해...
	m_pDebugSpawnMesh = new LineSphere(m_pDebugObject, pd3dDevice, pd3dCommandList, XMFLOAT4(0, 0, 1, 0), m_SpawnRange, m_SpawnRange);
}

Monster::~Monster()
{

}
 
void Monster::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	if (RENDER_DEBUG)
	{
		m_pMyBOBox->Render(pd3dCommandList);
		m_pDebugSpawnMesh->Render(pd3dCommandList, isGBuffers);
		m_RecognitionRange->RenderDebug(pd3dCommandList);
	}

	m_pHaep->UpdateShaderVariable(pd3dCommandList);
	m_pTexture->UpdateShaderVariable(pd3dCommandList, 0);
	m_pLoadObject->Render(pd3dCommandList, isGBuffers);

	RenderHpStatus(pd3dCommandList, isGBuffers);
}

void Monster::RenderForShadow(ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_pHaep->UpdateShaderVariable(pd3dCommandList);
	m_pTexture->UpdateShaderVariable(pd3dCommandList, 0);
	m_pLoadObject->RenderForShadow(pd3dCommandList);
}

void Monster::ReleaseMembers()
{
	if (m_pDebugSpawnMesh)
	{
		m_pDebugSpawnMesh->ReleaseObjects();
		delete m_pDebugSpawnMesh;
		m_pDebugSpawnMesh = nullptr;
	}
	if (m_MonsterHP)
	{
		m_MonsterHP->ReleaseObjects();
		delete m_MonsterHP;
		m_MonsterHP = nullptr;
	}
	if (m_RecognitionRange)
	{
		m_RecognitionRange->ReleaseObjects();
		delete m_RecognitionRange;
		m_RecognitionRange = nullptr;
	}
	if(m_pDebugObject)
	{
		m_pDebugObject->ReleaseObjects();
		delete m_pDebugObject;
		m_pDebugObject = nullptr;
	}
	if (m_pTexture)
	{
		m_pTexture->ReleaseObjects();
		delete m_pTexture;
		m_pTexture = nullptr;
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
	if (m_MonsterStatus)
	{
		m_MonsterStatus->ReleaseObjects();
		delete m_MonsterStatus;
		m_MonsterStatus = nullptr;
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
	if (m_MonsterHP) m_MonsterHP->ReleaseUploadBuffers();
	if (m_pDebugSpawnMesh) m_pDebugSpawnMesh->ReleaseUploadBuffers();
	if (m_pTexture) m_pTexture->ReleaseUploadBuffers();
	if (m_pLoadObject) m_pLoadObject->ReleaseUploadBuffers();
	if (m_MonsterModel)m_MonsterModel->ReleaseUploadBuffers();
	if (m_pMyBOBox)m_pMyBOBox->ReleaseUploadBuffers();
}
 

void Monster::SubstractHP(int sub)
{
	m_CurrAnimation = GetAnimationHitID();
	m_pLoadObject->SetTrackAnimationSet(0, m_CurrAnimation);
	
	m_MonsterStatus->m_HP -= sub;
	std::cout << m_MonsterStatus->m_HP << std::endl;
}

void Monster::Animate(float fElapsedTime)
{
	// animate 이전에 현재 설정된 애니메이션 수행하도록 설정
	SetTrackAnimationSet();

	// 반드시 트랜스폼 업데이트..! 
	m_Transform.Update(fElapsedTime);
	 
	m_pLoadObject->m_xmf4x4ToParent = m_Transform.GetWorldMatrix();
	
	m_pLoadObject->Animate(fElapsedTime); 
}
 
void Monster::RenderHpStatus(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{ 
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_UIWORLD, isGBuffers);

	// set look at.... 빌보드 처리...
	XMFLOAT4X4 uiWorld = m_Transform.GetWorldMatrix();
	uiWorld._42 += 200;

	XMFLOAT3 xmf3Position(uiWorld._41, uiWorld._42, uiWorld._43);

	XMFLOAT4X4 mtxLookAt = Matrix4x4::LookAtLH(xmf3Position, MainCameraMgr::GetMainCamera()->GetTransform().GetPosition(), XMFLOAT3(0, 1, 0));
	uiWorld._11 = mtxLookAt._11; uiWorld._12 = mtxLookAt._21; uiWorld._13 = mtxLookAt._31;
	uiWorld._21 = mtxLookAt._12; uiWorld._22 = mtxLookAt._22; uiWorld._23 = mtxLookAt._32;
	uiWorld._31 = mtxLookAt._13; uiWorld._32 = mtxLookAt._23; uiWorld._33 = mtxLookAt._33;
	// set look at....

	float percentage = float(m_MonsterStatus->m_HP) / 1000.f * 100.f;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &percentage, 0);

	m_MonsterHP->Render(pd3dCommandList, uiWorld);
}

void Monster::SetTrackAnimationSet()
{
	if (m_CurrAnimation != m_PrevAnimation)
	{
		m_pLoadObject->SetTrackAnimationSet(0, m_CurrAnimation);
		m_PrevAnimation = m_CurrAnimation;
	}
}

void Monster::Move(const XMFLOAT3 & xmf3Shift)
{
	m_Transform.Move(xmf3Shift);
}

void Monster::Rotate(float x, float y, float z)
{
	m_Transform.Rotate(x, y, z);
	m_pMyBOBox->Rotate(m_MonsterMovement->m_fRoll, m_MonsterMovement->m_fYaw, m_MonsterMovement->m_fPitch);
}

void Monster::SetAnimationState(int state)
{
	m_CurrAnimation = state;
	m_pLoadObject->SetTrackAnimationSet(0, m_CurrAnimation); 
}
 