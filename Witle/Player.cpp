#include "stdafx.h"
#include "MyBOBox.h" 
#include "Shader.h"
#include "Object.h"
#include "ShaderManager.h" 
#include "Transform.h"
#include "GameTimer.h"
#include "FollowCam.h"
#include "Terrain.h"
#include "Player.h"

#define ANIMATION_IDLE 0
#define ANIMATION_FORWARD 1
#define ANIMATION_BACKWARD 2
#define ANIMATION_LEFT 3
#define ANIMATION_RIGHT 4

void Player::OnPlayerUpdateCallback(float fTimeElapsed)
{
	if (!m_pPlayerUpdatedContext) return;

	Terrain *pTerrain = (Terrain *)m_pPlayerUpdatedContext;
	XMFLOAT3 xmf3Scale = pTerrain->GetScale();
	XMFLOAT3 xmf3PlayerPosition = m_Transform.GetPosition();
	int z = (int)(xmf3PlayerPosition.z / xmf3Scale.z);
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight = pTerrain->GetHeight(xmf3PlayerPosition.x, xmf3PlayerPosition.z, bReverseQuad);
	fHeight += 1.f;
	if (xmf3PlayerPosition.y < fHeight)
	{
		XMFLOAT3 xmf3PlayerVelocity = GetVelocity();
		xmf3PlayerVelocity.y = 0.0f;
		m_xmf3Velocity = xmf3PlayerVelocity;
		xmf3PlayerPosition.y = fHeight;
		m_Transform.SetPosition(xmf3PlayerPosition);
	}
}
XMFLOAT3 Player::CalculateAlreadyVelocity(float fTimeElapsed)
{
	XMFLOAT3 AlreadyVelocity = Vector3::Add(m_xmf3Velocity, m_xmf3Gravity);
	float fLength = sqrtf(AlreadyVelocity.x * AlreadyVelocity.x + AlreadyVelocity.z * AlreadyVelocity.z);
	float fMaxVelocityXZ = m_fMaxVelocityXZ;
	if (fLength > m_fMaxVelocityXZ)
	{
		AlreadyVelocity.x *= (fMaxVelocityXZ / fLength);
		AlreadyVelocity.z *= (fMaxVelocityXZ / fLength);
	}
	float fMaxVelocityY = m_fMaxVelocityY;
	fLength = sqrtf(AlreadyVelocity.y * AlreadyVelocity.y);
	if (fLength > m_fMaxVelocityY) AlreadyVelocity.y *= (fMaxVelocityY / fLength);

	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(AlreadyVelocity, fTimeElapsed, false);

	return xmf3Velocity;
}
BoundingOrientedBox Player::CalculateAlreadyBoundingBox(float fTimeElapsed)
{
	XMFLOAT3 AlreadyVelocity = CalculateAlreadyVelocity(fTimeElapsed);
	BoundingOrientedBox AlreadyBBox = m_MyBOBox->GetBOBox();
	AlreadyBBox.Center = Vector3::Add(AlreadyBBox.Center, AlreadyVelocity);
	return AlreadyBBox;
}
XMFLOAT3 Player::CalculateAlreadyPosition(float fTimeElapsed)
{
	XMFLOAT3 AlreadyVelocity = CalculateAlreadyVelocity(fTimeElapsed);
	XMFLOAT3 AlreadyPosition = m_Transform.GetPosition();
	AlreadyPosition = Vector3::Add(AlreadyPosition, AlreadyVelocity);
	return AlreadyPosition;
}

//void Player::OnCameraUpdateCallback(float fTimeElapsed, Camera* pCamera)
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
 

Player::Player(const std::string & entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
	: GameObject(entityID)
{ 
	CLoadedModelInfo *pPlayerModel = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Angrybot.bin", NULL);
	m_pLoadObject = pPlayerModel->m_pModelRootObject;

	m_pLoadObject->m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, 1, pPlayerModel);
	m_pLoadObject->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);

	XMFLOAT3 center{ 0.f, 75.f, 0.f };
	XMFLOAT3 extents{ 25.f, 75.f, 25.f };
	m_MyBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, center, extents);
	 
	SetUpdatedContext(pContext); 
}

Player::~Player()
{
	m_pPlayerUpdatedContext = nullptr;
	m_pCameraUpdatedContext = nullptr;
}

void Player::Update(float fElapsedTime)
{ 
	// 이동량을 계산한다. 
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, m_xmf3Gravity);
	float fLength = sqrtf(m_xmf3Velocity.x * m_xmf3Velocity.x + m_xmf3Velocity.z * m_xmf3Velocity.z);
	float fMaxVelocityXZ = m_fMaxVelocityXZ;
	if (fLength > m_fMaxVelocityXZ)
	{
		m_xmf3Velocity.x *= (fMaxVelocityXZ / fLength);
		m_xmf3Velocity.z *= (fMaxVelocityXZ / fLength);
	}
	float fMaxVelocityY = m_fMaxVelocityY;
	fLength = sqrtf(m_xmf3Velocity.y * m_xmf3Velocity.y);
	if (fLength > m_fMaxVelocityY) m_xmf3Velocity.y *= (fMaxVelocityY / fLength);

	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(m_xmf3Velocity, fElapsedTime, false);
	
	Move(xmf3Velocity); // 이동량만큼 움직인다.
	 
	// 플레이어 콜백
	// OnPlayerUpdateCallback(fElapsedTime);
	 
	// 이동량 줄어든다.
	//fLength = Vector3::Length(m_xmf3Velocity);
	//float fDeceleration = (m_fFriction * fElapsedTime); // 감소량
	//if (fDeceleration > fLength) fDeceleration = fLength;
	//m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Velocity, -fDeceleration, true));

	//m_pLoadObject->SetTrackAnimationSet(0, ::IsZero(fLength) ? 0 : 1); // 만약에 fLength가 0이 아니라면... 즉 움직인다면...
}

void Player::Animate(float fElapsedTime)
{
	// 반드시 트랜스폼 업데이트..! 
	m_Transform.Update(fElapsedTime);

	//// 위치가 안맞아서 재조정 
	m_pLoadObject->m_xmf4x4ToParent =  
		Matrix4x4::Multiply(XMMatrixRotationX(-90.0f), m_Transform.GetWorldMatrix());
	 
	if(m_pLoadObject) m_pLoadObject->Animate(fElapsedTime);
}

void Player::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
#ifdef _SHOW_BOUNDINGBOX
	m_MyBOBox->Render(pd3dCommandList, m_Transform.GetWorldMatrix());
#endif // _SHOW_BOUNDINGBOX

 	if(m_pLoadObject) m_pLoadObject->Render(pd3dCommandList);
}
 
void Player::SetTrackAnimationSet(ULONG dwDirection)
{ 
	m_pLoadObject->SetTrackAnimationSet(0, ANIMATION_IDLE);

	if (dwDirection & DIR_FORWARD)	m_pLoadObject->SetTrackAnimationSet(0, ANIMATION_FORWARD);
	if (dwDirection & DIR_BACKWARD) m_pLoadObject->SetTrackAnimationSet(0, ANIMATION_BACKWARD);
	if (dwDirection & DIR_RIGHT) m_pLoadObject->SetTrackAnimationSet(0, ANIMATION_RIGHT);
	if (dwDirection & DIR_LEFT) m_pLoadObject->SetTrackAnimationSet(0, ANIMATION_LEFT); 
}

void Player::Move(const XMFLOAT3 & xmf3Shift)
{
	m_Transform.Move(xmf3Shift);
	m_MyBOBox->Move(xmf3Shift);
}

void Player::MoveVelocity(const XMFLOAT3 & xmf3Shift)
{
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
}

void Player::Rotate(float x, float y, float z)
{
	m_Transform.Rotate(x, y, z);
 	m_MyBOBox->Rotate(m_fRoll, m_fYaw, m_fPitch);
}
