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
// GameBase /////////////////////

// DebugMesh /////////////////////
#include "MyBOBox.h" 
#include "LineSphere.h"
// DebugMesh /////////////////////

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

//void Monster::OnCameraUpdateCallback(float fTimeElapsed, Camera* pCamera)
//{
//	if (!m_pCameraUpdatedContext) return;
//	
//	Terrain *pTerrain = (Terrain *)m_pCameraUpdatedContext;
//	XMFLOAT3 xmf3Scale = pTerrain->GetScale();
//	XMFLOAT3 xmf3CameraPosition = pCamera->GetOwner()->GetTransform().GetPosition();
//	int z = (int)(xmf3CameraPosition.z / xmf3Scale.z);
//	bool bReverseQuad = ((z % 2) != 0);
//	float fHeight = pTerrain->GetHeight(xmf3CameraPosition.x, xmf3CameraPosition.z, bReverseQuad);
//	fHeight = fHeight + 5.f;
//	if (xmf3CameraPosition.y <= fHeight)
//	{
//		xmf3CameraPosition.y = fHeight;
//		pCamera->GetOwner()->GetTransform().SetPosition(xmf3CameraPosition);
//		static_cast<FollowCam *>(pCamera)->SetLookAt(m_Transform.GetPosition());
//	}
//}


Monster::Monster(const std::string & entityID, const XMFLOAT3& SpawnPoint, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature)
	: m_SpawnPoint(SpawnPoint), GameObject(entityID)
{
	m_RecognitionRange = new RecognitionRange(this, 500.f, 3.f);
	m_RecognitionRange->CreateDebugMesh(pd3dDevice, pd3dCommandList);

	XMFLOAT3 extents{ 25.f, 75.f, 25.f };
	m_pMyBOBox = new MyBOBox(this, pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 75.F, 0.F }, extents);

	m_MonsterStatus = new MonsterStatus(this, pd3dDevice, pd3dCommandList);
	m_MonsterMovement = new MonsterMovement(this);
	 
	// 디버그용
	m_pDebugObject = new EmptyGameObject("SpawnPosition");
	m_pDebugObject->GetTransform().SetPosition(SpawnPoint);
	m_pDebugObject->GetTransform().Update(0.f); // position update위해...
	m_pDebugSpawnMesh = new LineSphere(m_pDebugObject, pd3dDevice, pd3dCommandList, m_SpawnRange, m_SpawnRange);
}

Monster::~Monster()
{

}

void Monster::ReleaseMembers()
{ 
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
	if (m_pDebugSpawnMesh) m_pDebugSpawnMesh->ReleaseUploadBuffers();
	if (m_pTexture) m_pTexture->ReleaseUploadBuffers();
	if (m_pLoadObject) m_pLoadObject->ReleaseUploadBuffers();
	if (m_MonsterModel)m_MonsterModel->ReleaseUploadBuffers();
	if (m_pMyBOBox)m_pMyBOBox->ReleaseUploadBuffers();
}
 

void Monster::SubstractHP(int sub)
{
	m_CurrAnimation = ANIMATION_BEATTACKED.ID;
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

	//// 위치가 안맞아서 재조정  
	float fPitch = -90.f;
	float fYaw = 0.f;
	float fRoll = 0.f;
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	m_pLoadObject->m_xmf4x4ToParent = Matrix4x4::Multiply(mtxRotate, m_Transform.GetWorldMatrix());
	
	m_pLoadObject->Animate(fElapsedTime); 
}

void Monster::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{  
	m_pMyBOBox->Render(pd3dCommandList); 
	m_pDebugSpawnMesh->Render(pd3dCommandList);
	m_RecognitionRange->RenderDebug(pd3dCommandList);

	m_pHaep->UpdateShaderVariable(pd3dCommandList);
	m_pTexture->UpdateShaderVariable(pd3dCommandList, 0);
	m_pLoadObject->Render(pd3dCommandList);
}

void Monster::RenderHpStatus(ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_MonsterStatus->Render(pd3dCommandList);
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
 